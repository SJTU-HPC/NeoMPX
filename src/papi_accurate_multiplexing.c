#include "papi.h"
#include "papi_accurate_multiplexing.h"
#include "papi_internal.h"
#include "papi_vector.h"
#include "dataprocessing_utils.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <math.h>

#define PAPI_MPX_MAX(A, B) ((A > B) ? A : B)

/* variables to communicate between two threads. */
pthread_cond_t cond_stat = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER; 
pthread_barrier_t barrier_start;
int sampling = 1;

/*** general helper functions in this file ***/
static pid_t gettid()
{
    return syscall(SYS_gettid);
}

static void signal_stop_sampling(int sig)
{
    fprintf(stderr, "[log]: receiving signal %d to stop sampling.\n", sig);
    sampling = 0;
}

static int cmp(const void *a, const void *b)
{
    long long x = *(long long*)a;
    long long y = *(long long*)b;

    if(x > y)
        return 1;
    else if(x == y)
        return 0;
    else
        return -1;
}

static void cas_div(double *seq_cas, long long *seq, int len)
{
    int i;

    for(i = 0; i < len-1; ++i)
    {
        if(seq[i] != 0)
            seq_cas[i] = (double)seq[i+1] / seq[i];
        else
            seq_cas[i] = 1.0;
    }
    seq_cas[len-1] = 1.0;
}

static double time_to_time(double t, double PMU_INDEX)
{
    if(t == 1.0)
        return PMU_INDEX;

    double new_time = PMU_INDEX * (t-1) / log(t);
    return new_time;
}

static double dcam_raw_to_est(long long a, long long b, double PMU_INDEX)
{
    if(a == b)
        return a * PMU_INDEX;

    double m, acc;
    int i;

    if(a == 0)
    {
        m = pow(b, 1.0 / (PMU_INDEX - 1 + 1e-10));
        acc = 1.0;
        for(i = 0; i < (int)PMU_INDEX - 2; ++i)
        {
            acc += pow(m, i+1);
        }
    }
    else if(b == 0)
    {
        m = pow(1.0 / a, 1.0 / (PMU_INDEX - 1 + 1e-10));
        acc = (double)a;
        for(i = 0; i < (int)PMU_INDEX - 1; ++i)
        {
            acc += (double)a * pow(m, i+1);
        }
    }
    else
    {
        m = pow((double)b/(double)a, 1.0 / PMU_INDEX);
        acc = (double)a;
        for(i = 0; i < (int)PMU_INDEX - 1; ++i)
        {
            acc += (double)a * pow(m, i+1);
        }
        double rest_index = PMU_INDEX - (int)PMU_INDEX;
        acc += rest_index * (double)a * pow(m, PMU_INDEX);
    }

    return acc;
}

static double dira_raw_to_est(long long a, long long b, double PMU_INDEX)
{
    double m, acc;
    int i;

    m = (double)(b-a) / PMU_INDEX;
    acc = a;

    for(i = 0; i < (int)PMU_INDEX - 1; ++i)
    {
        acc += a + m * (i+1);
    }
    double rest_index = PMU_INDEX - (int)PMU_INDEX;
    acc += rest_index * (a + m * PMU_INDEX);

    return acc;
}

/*** postprocessing functions in this file ***/
static void te_fix(long long *Te, int nsamples, int kmeans_clusters, void *tmpbuffer)
{
    int most_common_mode, i;
    
    long long *diff_te = (long long*)tmpbuffer; // len = nsamples-1
    KMeansResult *kms = (KMeansResult *)(diff_te + (nsamples-1));
    void *kmeans_buffer = (void *)(kms+1);

    diff(diff_te, Te, nsamples);
    KMeans(kms, diff_te, nsamples-1, kmeans_clusters, kmeans_buffer);
    most_common_mode = Mode(kms);

    for(i = 0; i < nsamples - 1; ++i)
    {
        if(kms->label[i] != most_common_mode)
            diff_te[i] = kms->center[most_common_mode];
    }

    CumSum(Te, diff_te, nsamples);
}

static void estimation(long long *Count, long long *Te, long long *Tr, int nsamples)
{
    int t;

    for(t = 0; t < nsamples; ++t)
    {
        if(Tr[t] != 0)
        {
            long long scale = Te[t] * 100LL / Tr[t];
            Count[t] = Count[t] * scale / 100LL;
        }
        else
        {
            Count[t] = 0;
        }
    }
}

static void cam(long long *Count, long long *Te, long long *Tr, int nsamples, void *tmpbuffer)
{
    int difflen = nsamples - 1;
    int i, j;
    
    long long *diff_count = (long long*)tmpbuffer;
    long long *diff_te = diff_count + difflen;
    long long *diff_tr = diff_te + difflen;
    double *diff_count_cas = (double*)(diff_tr + difflen);

    diff(diff_count, Count, nsamples);
    diff(diff_te, Te, nsamples);
    diff(diff_tr, Tr, nsamples);

    cas_div(diff_count_cas, diff_count, difflen);

    for(i = 0; i < difflen - 1; ++i)
    {
        double PMU_INDEX;
        if(diff_tr[i] != 0)
            PMU_INDEX = (double)diff_te[i] / diff_tr[i];
        else
            PMU_INDEX = 1.0;

        diff_count_cas[i] = time_to_time(diff_count_cas[i], PMU_INDEX);

        diff_count[i] = diff_count_cas[i] * diff_count[i];
    }

    if(diff_tr[difflen-1] != 0)
        diff_count[difflen-1] = diff_count[difflen-1] * (double)diff_te[difflen-1] / diff_tr[difflen-1]; 

    CumSum(Count, diff_count, nsamples);
}

static void tam(long long *Count, long long *Te, long long *Tr, int nsamples, void *tmpbuffer)
{
    int difflen = nsamples - 1;
    int i, j;

    long long *diff_count = (long long*)tmpbuffer;
    long long *diff_te = diff_count + difflen;
    long long *diff_tr = diff_te + difflen;
    double *alpha = (double*)(diff_tr + difflen);

    diff(diff_count, Count, nsamples);
    diff(diff_te, Te, nsamples);
    diff(diff_tr, Tr, nsamples);

    for(i = 0; i < difflen; ++i)
    {
        if(diff_tr[i] != 0)
            alpha[i] = (double)diff_te[i] / diff_tr[i];
        else
            alpha[i] = 1.0;
    }

    alpha[0] = alpha[0] * diff_count[0] / 2;
    for(i = 0; i < difflen - 1; ++i)
        alpha[i+1] = (diff_count[i] + diff_count[i+1]) * alpha[i+1] / 2.0 + (diff_count[i] - diff_count[i+1]) / 2.0;

    for(i = 0; i < difflen; ++i)
        diff_count[i] = (long long)alpha[i];

    CumSum(Count, diff_count, nsamples);
}

static void dcam(long long *Count, long long *Te, long long *Tr, int nsamples, void *tmpbuffer)
{
    int difflen = nsamples - 1;
    int i, j;

    long long *diff_count = (long long*)tmpbuffer;
    long long *diff_te = diff_count + difflen;
    long long *diff_tr = diff_te + difflen;
    double *alpha = (double*)(diff_tr + difflen);

    diff(diff_count, Count, nsamples);
    diff(diff_te, Te, nsamples);
    diff(diff_tr, Tr, nsamples);

    for(i = 0; i < difflen; ++i)
    {
        if(diff_tr[i] != 0)
            alpha[i] = (double)diff_te[i] / diff_tr[i];
        else
            alpha[i] = 1.0;
    }

    for(i = 0; i < difflen - 1; ++i)
    {
        alpha[i] = dcam_raw_to_est(diff_count[i], diff_count[i+1], alpha[i]); 
    }
    alpha[difflen-1] = dcam_raw_to_est(diff_count[difflen-1], 0, alpha[difflen-1]);

    for(i = 0; i < difflen; ++i)
        diff_count[i] = (long long)alpha[i];

    CumSum(Count, diff_count, nsamples);
}

static void dira(long long *Count, long long *Te, long long *Tr, int nsamples, void *tmpbuffer)
{
    int difflen = nsamples - 1;
    int i, j;

    long long *diff_count = (long long*)tmpbuffer;
    long long *diff_te = diff_count + difflen;
    long long *diff_tr = diff_te + difflen;
    double *alpha = (double*)(diff_tr + difflen);

    diff(diff_count, Count, nsamples);
    diff(diff_te, Te, nsamples);
    diff(diff_tr, Tr, nsamples);

    for(i = 0; i < difflen; ++i)
    {
        if(diff_tr[i] != 0)
            alpha[i] = (double)diff_te[i] / diff_tr[i];
        else
            alpha[i] = 1.0;
    }

    for(i = 0; i < difflen - 1; ++i)
    {
        alpha[i] = dira_raw_to_est(diff_count[i], diff_count[i+1], alpha[i]); 
    }
    alpha[difflen-1] = dira_raw_to_est(diff_count[difflen-1], 0, alpha[difflen-1]);

    for(i = 0; i < difflen; ++i)
        diff_count[i] = (long long)alpha[i];

    CumSum(Count, diff_count, nsamples);
}

static void outlier_fix(long long *seq, int nsamples, void *tmpbuffer)
{
    int len = nsamples - 1;
    int low=0, high=0, low_flag=0, high_flag=0;
    int i;
    
    long long *diff_seq = (long long*)tmpbuffer;
    diff(diff_seq, seq, nsamples);

    for(i = 0; i < 6; ++i)
    {
        if((mostlessthan(diff_seq,i, len) == 1) && (high_flag == 0))
        {
            high = i;
            high_flag = 1;
        }
        if((mostlargerthan(diff_seq,-i, len) == 1) && (low_flag == 0))
        {
            low = -i;
            low_flag = 1;
        }
        if((low_flag == 1) && (high_flag == 1))
            break;
    }

    if(high_flag == 0)
        high = 5;
    if(low_flag == 0)
        low = -5;

    boundry_fix(diff_seq, high, low, len);

    CumSum(seq, diff_seq, nsamples);
}

static void missing_fix(long long *seq, int nsamples, int knn_k, void *tmpbuffer)
{
    int x, y, i, len=nsamples-1;
    long long *diff_seq = (long long*)tmpbuffer;
    long long *sort_seq = diff_seq + len;

    diff(diff_seq, seq, nsamples);

    memcpy(sort_seq, diff_seq, len*sizeof(long long));
    qsort(sort_seq, len, sizeof(long long), cmp);

    if(sort_seq[(len / 3)] > 0)
    {
        return;
    }

    x = knn_k / 2 + 1;
    y = len - 1 - knn_k / 2;
    for(i = 0; i < len; ++i)
    {
        if ((i < x) || (i > y))
            continue;
        if (diff_seq[i] == 0)
            diff_seq[i] = knn_mean(diff_seq, len, i, knn_k);
    }

    CumSum(seq, diff_seq, nsamples);
}

/*** thread helper functions in this file ***/
static int papi_mpx_add_event_func(int EventSet, int EventCode)
{
    EventSetInfo_t *ESI;
    int retval;
    
    // check Eventset existence
    ESI = _papi_hwi_lookup_EventSet(EventSet);
    if (ESI == NULL)
        return PAPI_ENOEVST;

    // check EventCode validity
    if (EventCode & PAPI_NATIVE_MASK == 0)
        return PAPI_EINVAL;
    
    retval = _papi_hwi_add_event(ESI, EventCode);
    return retval;
}

static int papi_mpx_sampling_func(int EventSet, int interval)
{
    EventSetInfo_t *ESI;
    int num_events;
    FILE *fout, *ftest;
    long long *read_buf;
    hwd_context_t *context;
    int cidx, i;
    struct timespec start, end, sampling_interval;

    ESI = _papi_hwi_lookup_EventSet(EventSet);
    cidx = ESI->CmpIdx;
    num_events = ESI->NumberOfEvents;
    //step 1: open file for raw data, malloc buffer for events
    ftest = fopen("test.bin", "wb+");
    fout = fopen("raw.bin", "wb+");
    read_buf = (long long*)malloc(num_events*3*sizeof(long long));

    //step 2: test minimum sampling period for 1000 times
    context = _papi_hwi_get_context(ESI, NULL);
    _papi_hwd[cidx]->start(context, ESI->ctl_state);
    clock_gettime(CLOCK_MONOTONIC, &start);
    _papi_hwd[cidx]->read_raw(context, ESI->ctl_state, read_buf);
    fwrite(read_buf, sizeof(long long), 3*num_events, ftest);
    sleep(5);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double read_time = ((end.tv_sec - start.tv_sec - 5) * 1e9 + (end.tv_nsec - start.tv_nsec));
    if(read_time > (double)interval)
    {
        fprintf(stderr, "[warning]: preset sampling interval(%lf ms) is smaller than read time(%lf ms), setting it to 10*read_time.\n", (double)interval*1e-6, read_time*1e-6);
        interval = read_time * 10;
    }
    sampling_interval.tv_sec = interval / (int)1e9;
    sampling_interval.tv_nsec = interval % (int)1e9;
    fclose(ftest);
    _papi_hwd[cidx]->stop(context, ESI->ctl_state);

    //step 3: reset counter and send signal
    ESI->state ^= PAPI_STOPPED;
    ESI->state |= PAPI_RUNNING;
    _papi_hwd[cidx]->reset(context, ESI->ctl_state);

    pthread_barrier_wait(&barrier_start);

    _papi_hwd[cidx]->start(context, ESI->ctl_state); 

    //step 4: periodically sampling
    while(1)
    {
        nanosleep(&sampling_interval, NULL);
        
        if(sampling == 1)
        {
            double rs;
            clock_gettime(CLOCK_MONOTONIC, &start);
            _papi_hwd[cidx]->read_raw(context, ESI->ctl_state, read_buf);
            fwrite(read_buf, sizeof(long long), 3*num_events, fout);
            clock_gettime(CLOCK_MONOTONIC, &end);

            rs = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);

            if(rs <= (double)interval)
            {
                sampling_interval.tv_sec = (int)(interval - rs) / (int)1e9;
                sampling_interval.tv_nsec = (int)(interval - rs) % (int)1e9;
            }
            else
            {
                fprintf(stderr, "[warning]: reading time(%lf ms) is larger than sampling interval(%lf ms), the result could be inaccurate!\n", rs/1e6, interval/1e6);
                sampling_interval.tv_sec = (int)(rs) / (int)1e9;
                sampling_interval.tv_nsec = (int)(rs) % (int)1e9;
            }
        }
        else
        {
            _papi_hwd[cidx]->read_raw(context, ESI->ctl_state, read_buf);
            fwrite(read_buf, sizeof(long long), 3 * num_events, fout);
            break;
        }
    }

    fclose(fout);
    free(read_buf);

    return PAPI_OK;
}

static size_t papi_mpx_post_processing_get_buf_size(int num_events, int nsamples, int kmeans_clusters, PAPI_MPX_postprocessing_method method)
{
    size_t max_buf_size;
    
    //read buf
    size_t read_buf_size = num_events * 3 * nsamples * sizeof(long long);

    //Te fix buf
    size_t Te_fix_buf_size = (nsamples - 1) * sizeof(long long); // diff size
    Te_fix_buf_size += sizeof(KMeansResult) + (nsamples - 1) * sizeof(int) + kmeans_clusters * 2 * sizeof(double); // kmeans size
    Te_fix_buf_size += (nsamples - 1) * sizeof(int) + kmeans_clusters * sizeof(double) + kmeans_clusters * sizeof(int) + kmeans_clusters * (nsamples - 1) * sizeof(long long); // cluster buffer

    //Outlier fix buf
    size_t outlier_fix_buf_size = (nsamples-1) * sizeof(long long); // diff_seq

    //Missing fix buf
    size_t missing_fix_buf_size = 2 * (nsamples-1) * sizeof(long long); // diff_seq and seq_sort

    //New estimation buf
    size_t cam_buf_size = 3 * (nsamples-1) * sizeof(long long) + (nsamples-1) * sizeof(double);

    max_buf_size = read_buf_size;
    if(method & PAPI_MPX_POST_TE)
        max_buf_size = PAPI_MPX_MAX(max_buf_size, Te_fix_buf_size);
    if(method & PAPI_MPX_POST_CAM || method & PAPI_MPX_POST_TAM || method & PAPI_MPX_POST_DCAM || method & PAPI_MPX_POST_DIRA )
        max_buf_size = PAPI_MPX_MAX(max_buf_size, cam_buf_size);
    if(method & PAPI_MPX_POST_OUTLIER)
        max_buf_size = PAPI_MPX_MAX(max_buf_size, outlier_fix_buf_size);
    if(method & PAPI_MPX_POST_MISSING)
        max_buf_size = PAPI_MPX_MAX(max_buf_size, missing_fix_buf_size);

    return max_buf_size;
}

static int papi_mpx_post_processing_func(int EventSet, long long *values, PAPI_MPX_postprocessing_method method, PAPI_MPX_data_flag data_flag)
{
    EventSetInfo_t *ESI;
    FILE *fraw;
    int num_events, nsamples, t, ne;
    long long sizefraw;
    long long *read_buf, *Count, *Te, *Tr;

    //set parameters
    int kmeans_clusters = 3;
    int knn_k = 5;

    ESI = _papi_hwi_lookup_EventSet(EventSet);
    num_events = ESI->NumberOfEvents;
    
    //calculate nsamples
    fraw = fopen("raw.bin", "rb");
    fseek(fraw, 0, SEEK_END);
    sizefraw = ftell(fraw);
    nsamples = sizefraw / (sizeof(long long)* num_events * 3);
    fseek(fraw, 0, SEEK_SET);

    // calculate tmp buffer size of post processing
    size_t buf_size = papi_mpx_post_processing_get_buf_size(num_events, nsamples, kmeans_clusters, method);
    fprintf(stderr, "[log]: post processing buf size = %lf MB.\n", (double)buf_size/(1024*1024));
    void *buffer = (void*)malloc(buf_size);
    read_buf = (long long*)buffer;
    
    //malloc buffer for arrays and read fraw
    Count = (long long *)malloc(num_events*nsamples*sizeof(long long));
    Te = (long long *)malloc(num_events*nsamples*sizeof(long long));
    Tr = (long long *)malloc(num_events*nsamples*sizeof(long long));
    fread(read_buf, sizeof(long long), 3*num_events*nsamples, fraw);
    fclose(fraw);

    //rearrange arrays
    for(t = 0; t < nsamples; ++t)
    {
        for(ne = 0; ne < num_events; ++ne)
        {
            Count[ne*nsamples+t] = read_buf[t*num_events*3+ne*3+0];
            Te[ne*nsamples+t] = read_buf[t*num_events*3+ne*3+1];
            Tr[ne*nsamples+t] = read_buf[t*num_events*3+ne*3+2];
        }
    }

    memset(buffer, 0, sizeof(buf_size));

    for(ne = 0; ne < num_events; ++ne)
    {
        //[1] fix te
        if(method & PAPI_MPX_POST_TE)
        {
            fprintf(stderr, "Postprocessing with TE fix\n");
            te_fix(Te+ne*nsamples, nsamples, kmeans_clusters, buffer);
        }
        
        //[2] estimation
        if(method & PAPI_MPX_POST_CAM)
        {
            fprintf(stderr, "Estimating with CAM\n");
            cam(Count+ne*nsamples, Te+ne*nsamples, Tr+ne*nsamples, nsamples, buffer);
        }
        else if(method & PAPI_MPX_POST_TAM)
        {
            fprintf(stderr, "Estimating with TAM\n"); 
            tam(Count+ne*nsamples, Te+ne*nsamples, Tr+ne*nsamples, nsamples, buffer);
        }
        else if(method & PAPI_MPX_POST_DCAM)
        {
            fprintf(stderr, "Estimating with DCAM\n"); 
            dcam(Count+ne*nsamples, Te+ne*nsamples, Tr+ne*nsamples, nsamples, buffer); 
        }
        else if(method & PAPI_MPX_POST_DIRA)
        {
            fprintf(stderr, "Estimating with DIRA\n"); 
            dira(Count+ne*nsamples, Te+ne*nsamples, Tr+ne*nsamples, nsamples, buffer); 
        } 
        else
        {
            fprintf(stderr, "Estimating with PAPI ORI\n");
            estimation(Count+ne*nsamples, Te+ne*nsamples, Tr+ne*nsamples, nsamples);
        }

        //[3] fix outlier
        if(method & PAPI_MPX_POST_OUTLIER)
        {
            fprintf(stderr, "Postprocessing with outlier fix\n");
            outlier_fix(Count+ne*nsamples, nsamples, buffer);
        }

        //[4] fix missing value
        if(method & PAPI_MPX_POST_MISSING)
        {
            fprintf(stderr, "Postprocessing with missing value fix\n");
            missing_fix(Count+ne*nsamples, nsamples, knn_k, buffer);
        }
    }

    //Currently use the last count value for the output
    for(ne = 0; ne < num_events; ++ne)
    {
        values[ne] = Count[ne*nsamples+(nsamples-1)];
    }

    if((data_flag & PAPI_MPX_SAVING_RAW) == 0)
    {
        remove("raw.bin");
    }
    if(data_flag & PAPI_MPX_SAVING_FIX)
    {
        FILE *fix = fopen("fix.bin", "wb+");
        fwrite(Count, sizeof(long long), num_events*nsamples, fix);
        fclose(fix);
    }

    free(Count);
    free(Te);
    free(Tr);
    free(buffer);
    remove("test.bin");

    return PAPI_OK;
}

/* thread function of accurate multiplexing */
static void *papi_accurate_thread_func(void *p)
{
    pid_t mypid = gettid();
    int run = 1;
    int retval;

    PAPI_MPX_attr *mpx = (PAPI_MPX_attr*)p;

    while(run)
    {
        pthread_mutex_lock(&mut);
        while(mpx->status == PAPI_MPX_STATUS_WAIT)
        {
            pthread_cond_wait(&cond_stat, &mut);
        }

        switch(mpx->status)
        {
            case PAPI_MPX_STATUS_INIT:
            {
                fprintf(stderr, "[log]: sampling thread %d initializes. Waiting for signal.\n", mypid);
                mpx->childpid = mypid;
                mpx->status = PAPI_MPX_STATUS_WAIT;
                break;
            }
            case PAPI_MPX_STATUS_CREATE:
            {
                fprintf(stderr, "[log]: creating multiplexing eventset attach to thread %d.\n", mpx->mainpid);
                PAPI_create_eventset(mpx->EventSet);
                PAPI_assign_eventset_component(*mpx->EventSet, 0);
                PAPI_attach(*mpx->EventSet, mpx->mainpid);
                PAPI_set_multiplex(*mpx->EventSet);
                mpx->status = PAPI_MPX_STATUS_WAIT;
                break;
            }
            case PAPI_MPX_STATUS_ADD:
            {
                fprintf(stderr, "[log]: sampling thread %d adding events.\n", mypid);
                retval = papi_mpx_add_event_func(*mpx->EventSet,mpx->Event);
                if(retval == PAPI_ENOEVST)
                {
                    fprintf(stderr, "[error]: EventSet does not exist.\n");
                    run = 0;
                }
                else if(retval == PAPI_EINVAL)
                {
                    fprintf(stderr, "[error]: EventCode is invalid\n");
                    run = 0;
                }
                else
                {
                    mpx->status = PAPI_MPX_STATUS_WAIT;
                }
                break;
            }
            case PAPI_MPX_STATUS_START:
            {
                fprintf(stderr, "[log]: sampling thread %d starts sampling.\n", mypid);
                papi_mpx_sampling_func(*mpx->EventSet, mpx->sampling_interval);
                mpx->status = PAPI_MPX_STATUS_WAIT;
                break;
            }
            case PAPI_MPX_STATUS_STOP:
            {
                fprintf(stderr, "[log]: sampling thread %d stops and performs post-processing.\n", mypid);
                papi_mpx_post_processing_func(*mpx->EventSet, mpx->values, mpx->method, mpx->data_flag);
                mpx->status = PAPI_MPX_STATUS_WAIT;
                break;
            }
            case PAPI_MPX_STATUS_FINAL:
            {
                fprintf(stderr, "[log]: sampling thread %d finalizes\n", mypid);
                run = 0;
                break;
            }
            case PAPI_MPX_STATUS_WAIT:
            {
                fprintf(stderr, "[Error]: sampling thread %d entering wait error\n", mypid);
                run = 0;
                break; 
            }
            default:
            {
                fprintf(stderr, "[Error]: sampling thread %d receives an unexpected status\n", mypid);
                run = 0;
                break;
            }
        }
        pthread_mutex_unlock(&mut2);
        pthread_mutex_unlock(&mut);
    }

    pthread_exit(NULL);
}

/* initializing MPX sampling threads */
int PAPI_MPX_sampling_init(PAPI_MPX_attr *mpx_attr)
{
    //initialize MPX_attr variables
    mpx_attr->mainpid = gettid();
    mpx_attr->status = PAPI_MPX_STATUS_INIT;

    //initializing the barrier
    pthread_barrier_init(&barrier_start, NULL, 2);

    //create mpx threads    
    pthread_create(&mpx_attr->tsample, NULL, papi_accurate_thread_func, (void*)mpx_attr);

    pthread_mutex_lock(&mut2);

    return PAPI_OK;
}

int PAPI_MPX_finalize(PAPI_MPX_attr *mpx_attr)
{
    pthread_mutex_lock(&mut2);
    pthread_mutex_lock(&mut);
    mpx_attr->status = PAPI_MPX_STATUS_FINAL;
    if(mpx_attr->status != PAPI_MPX_STATUS_WAIT)
        pthread_cond_broadcast(&cond_stat);
    pthread_mutex_unlock(&mut);

    pthread_join(mpx_attr->tsample, NULL);

    return PAPI_OK;
}

/* add event into MPX eventset
 * currently only support native events
*/
int PAPI_MPX_add_event(PAPI_MPX_attr *mpx_attr, int *EventSet, int EventCode)
{
    pthread_mutex_lock(&mut2);
    pthread_mutex_lock(&mut);
    mpx_attr->EventSet = EventSet;
    mpx_attr->Event = EventCode;
    mpx_attr->status = PAPI_MPX_STATUS_ADD;
    if(mpx_attr->status != PAPI_MPX_STATUS_WAIT)
        pthread_cond_broadcast(&cond_stat);
    pthread_mutex_unlock(&mut);

    return PAPI_OK;
}

/* create eventset and attach it to the main thread*/
int PAPI_MPX_create_eventset(PAPI_MPX_attr *mpx_attr, int *EventSet)
{
    pthread_mutex_lock(&mut2);
    pthread_mutex_lock(&mut);
    mpx_attr->EventSet = EventSet;
    mpx_attr->status = PAPI_MPX_STATUS_CREATE;
    if(mpx_attr->status != PAPI_MPX_STATUS_WAIT)
        pthread_cond_broadcast(&cond_stat);
    pthread_mutex_unlock(&mut);

    return PAPI_OK;
}

/* start multiplexing sampling
 * interval: can be defined by the user. use -1 for default situation,the minimum sampling period.
 */
int PAPI_MPX_start(PAPI_MPX_attr *mpx_attr, int *EventSet, int interval)
{
    pthread_mutex_lock(&mut2);
    pthread_mutex_lock(&mut);
    mpx_attr->EventSet = EventSet;
    mpx_attr->sampling_interval = interval;
    mpx_attr->status = PAPI_MPX_STATUS_START;
    if(mpx_attr->status != PAPI_MPX_STATUS_WAIT)
        pthread_cond_broadcast(&cond_stat);
    pthread_mutex_unlock(&mut);

    // define signal to stop sampling
    signal(SIGUSR1, signal_stop_sampling);

    // wait for the the thread signal to execute source code.
    pthread_barrier_wait(&barrier_start);

    return PAPI_OK;
}

/* start multiplexing sampling
 * data_flag: flag to decide whether or not to store the sequence data
 */
int PAPI_MPX_stop(PAPI_MPX_attr *mpx_attr, int *EventSet, long long *values, PAPI_MPX_postprocessing_method method, PAPI_MPX_data_flag data_flag)
{
    //step 1: send signal to stop the sampling

    kill(mpx_attr->childpid, SIGUSR1);

    pthread_mutex_lock(&mut2);

    //step 2: go to the stop and post processing procedure
    pthread_mutex_lock(&mut);
    mpx_attr->EventSet = EventSet;
    mpx_attr->data_flag = data_flag;
    mpx_attr->method = method;
    mpx_attr->values = values;
    mpx_attr->status = PAPI_MPX_STATUS_STOP;
    if(mpx_attr->status != PAPI_MPX_STATUS_WAIT)
        pthread_cond_broadcast(&cond_stat);
    pthread_mutex_unlock(&mut); 

    return PAPI_OK;
}
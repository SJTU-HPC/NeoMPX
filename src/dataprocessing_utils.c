#include "dataprocessing_utils.h"

#include <math.h>
#include <string.h>

#define KMEANS_THRESHOLD 10E-6

/*** helper functions ***/
/* Distance between two point */
static inline double Distance(double a, double b)
{
    return a > b ? a - b : b - a;
}

/*Find the minimum element in the array
 *And return the index
 */
static int MinArrayIndex(double *a, int len)
{
    int ret = 0, i;
    double temp = a[0];

    for(i = 1; i < len; ++i)
    {
        if(temp > a[i])
        {
            temp = a[i];
            ret = i;
        }
    }

    return ret;
}

/*Find the maximum element in the array
 *And return the index
 */
static int MaxArrayIndex(int *a, int len)
{
    int ret = 0, i;
    double temp = a[0];

    for(i = 1; i < len; ++i)
    {
        if(temp < a[i])
        {
            temp = a[i];
            ret = i;
        }
    }

    return ret;
}

/* Get New Center of a group */
static double GetCenter(long long *p, int len)
{
    int i;
    double sum = 0;
    
    for(i = 0; i < len; ++i)
        sum += p[i];

    return sum / len;
}

/* Compare old center and new center */
int Equal(double *a, double *b, int k)
{
    int i;

    for (i = 0; i < k; ++i)
    {
        if (fabs(a[i] - b[i]) > KMEANS_THRESHOLD)
            return 0;
    }
    return 1;
}

static void Cluster(double *new_center, long long *p, int len, double *old_center, int k, void *buffer)
{
    int i, j;
    
    int *plabel = (int *)buffer;
    double *dist = (double *)(plabel + len);
    int *group_num = (int *)(dist + k);
    long long *group_member = (long long*)(group_num + k);

    for(i = 0; i < len; ++i)
    {
        for(j = 0; j < k; ++j)
        {
            dist[j] = Distance(p[i], old_center[j]);
        }

        plabel[i] = MinArrayIndex(dist, k);
    }

    for(i = 0; i < k; ++i)
    {
        int num = 0;
        for(j = 0; j < len; ++j)
        {
            if(plabel[j] == i)
            {
                group_member[i*len+num] = p[j];
                ++num;
            }
        }

        group_num[i] = num;
    }

    for(i = 0; i < k; ++i)
    {
        new_center[i] = GetCenter(group_member+i*len, group_num[i]);
    }
}

/* calculate the mean and standard deviation of a seq */
static void calculate_mean_and_std(double *mean, double *std, long long *seq, int len)
{
    double sum = 0;
    int i;

    for(i = 0; i < len; ++i)
        sum += seq[i];

    *mean = sum / len;

    if(std != NULL)
    {
        sum = 0;
        for(i = 0; i < len; ++i)
        {
            sum += (seq[i] - *mean) * (seq[i] - *mean);
        }
        sum /= len;
        *std = sqrt(sum);
    }
}

/*** utils functions ***/
void diff(long long *out, long long *in, int len)
{
    int i;
    for(i = 0; i < len - 1; ++i)
        out[i] = in[i+1] - in[i];
}

void KMeans(KMeansResult *kms, long long *p, int len, int k, void *buffer)
{
    double *old_center;
    int i, j, index, add_value;
    void *cluster_buffer;
    
    kms->label = (int *)buffer;
    kms->center = (double *)(kms->label + len);
    old_center = (double *)(kms->center + k);
    cluster_buffer = (void *)(old_center + k);
    kms->k = k;
    kms->nsample = len;
    
    //select the initial center
    index = 0;
    add_value = (len - 1) / (k - 1);
    for(i = 0; i < k; ++i)
    {
        old_center[i] = p[index];
        index += add_value;
    }

    while(1)
    {
        Cluster(kms->center, p, len, old_center, k, cluster_buffer);
        if(!Equal(kms->center, old_center, k))
        {
            memcpy(old_center, kms->center, k*sizeof(double));
        }
        else
            break;
    }

    double *dist = (double *)cluster_buffer;
    for(i = 0; i < len; ++i)
    {
        for(j = 0; j < k; ++j)
            dist[j] = Distance(p[i], kms->center[j]);

        kms->label[i] = MinArrayIndex(dist, k);
    }
}

int Mode(KMeansResult *kms)
{
    int i;
    int *counts = (int*)(kms->center+kms->k);
    memset(counts, 0, kms->k*sizeof(int));

    for(i = 0; i < kms->nsample; ++i)
        ++counts[kms->label[i]];

    return MaxArrayIndex(counts, kms->k);
}

void CumSum(long long *p, long long *diff_p, int len)
{
    int i;
    long long temp = diff_p[0] + p[0];
    p[1] = diff_p[0] + p[0];

    for(i = 2; i < len; ++i)
    {
        temp += diff_p[i-1];
        p[i] = temp;
    }
}

long long knn_mean(long long *seq, int len, int index, int knn_k)
{
    int start = 0, endd = len - 1;
    int x = (index - ceil(knn_k / 2.0));
    int y = (index + floor(knn_k / 2.0));
    int i;

    if (x >= 0)
        start = x;

    if (y <= endd)
        endd = y;

    long long sum = 0;;
    for (i = start; i<= endd; ++i)
        sum += seq[i];

    return sum / (endd - start + 1);
}

int mostlessthan(long long *seq, int index, int len)
{
    double mean, std, thre;
    int num=0, i;
    double rate;

    calculate_mean_and_std(&mean, &std, seq, len);
    thre = mean + index * std;
    
    for(i = 0; i < len; ++i)
    {
        if(seq[i] < thre)
            ++num;
    }

    rate = (double)num / len;

    if(rate >= 0.98)
        return 1;
    else
        return 0;
}

int mostlargerthan(long long *seq, int index, int len)
{
    double mean, std, thre;
    int num=0, i;
    double rate;

    calculate_mean_and_std(&mean, &std, seq, len);
    thre = mean + index * std;
    
    for(i = 0; i < len; ++i)
    {
        if(seq[i] > thre)
            ++num;
    }

    rate = (double)num / len;

    if(rate >= 0.98)
        return 1;
    else
        return 0;
}

void boundry_fix(long long *seq, int high, int low, int len)
{
    double mean, std, high_thre, low_thre;
    int num=0, i;
    double rate;

    calculate_mean_and_std(&mean, &std, seq, len);
    high_thre = mean + high * std;
    low_thre = mean + low * std;

    for(i = 0; i < len; ++i)
    {
        if(seq[i] > high_thre)
            seq[i] = high_thre;
        if(seq[i] < low_thre)
            seq[i] < low_thre;
    }
}
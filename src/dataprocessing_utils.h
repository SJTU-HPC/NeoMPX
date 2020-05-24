#ifndef DATAPROCESSING_UTILS_H
#define DATAPROCESSING_UTILS_H

typedef struct kmeansresult {
    int nsample;
    int k;
    int *label;
    double *center;
} KMeansResult;

/*** functions of array operation ***/
void diff(long long *out, long long *in, int len); // get the first order difference

void CumSum(long long *p, long long *diff_p, int len); // cumulatively sum first order difference

/*** functions of KMeans algorithm ***/
void KMeans(KMeansResult *kms, long long *p, int len, int k, void *buffer); // implement the kmeans algorithm

int Mode(KMeansResult *kms); // get the most common mode of kmeans

long long knn_mean(long long *seq, int len, int index, int knn_k); //calculate the mean by knn

/* utils for outlier fix */
int mostlessthan(long long *seq, int index, int len);

int mostlargerthan(long long *seq, int index, int len);

void boundry_fix(long long *seq, int high, int low, int len);

#endif
# NeoMPX : Characterizing and Improving Estimation for Multiplexing Hardware Counters
NeoMPX is an open-source MPX library for PAPI 5.7.0

# API Reference
```C
int PAPI_MPX_sampling_init(PAPI_MPX_attr *mpx_attr); // initializing MPX sampling threads

int PAPI_MPX_create_eventset(PAPI_MPX_attr *mpx_attr, int *EventSet); // create MPX eventset

int PAPI_MPX_add_event(PAPI_MPX_attr *mpx_attr, int *EventSet, int EventCode); // add event into MPX eventset

int PAPI_MPX_start(PAPI_MPX_attr *mpx_attr, int *EventSet, int interval); // start MPX sampling

int PAPI_MPX_stop(PAPI_MPX_attr *mpx_attr, int *EventSet, long long *values, PAPI_MPX_postprocessing_method method, PAPI_MPX_data_flag data_flag); // stop MPX sampling and post-processing

int PAPI_MPX_finalize(PAPI_MPX_attr *mpx_attr); // finalizing MPX sampling threads 
```

# Data Structure Reference
```C
/* flags for saving data */
typedef enum dataflag {
    PAPI_MPX_SAVING_NONE = 0x0,
    PAPI_MPX_SAVING_RAW = 0x1,
    PAPI_MPX_SAVING_FIX = 0x2
} PAPI_MPX_data_flag;

/* methods for postprocessing and estimation */
typedef enum postmethod {
    PAPI_MPX_POST_NONE = 0x0,
    PAPI_MPX_POST_TE = 0x1,
    PAPI_MPX_POST_OUTLIER = 0x2,
    PAPI_MPX_POST_MISSING = 0x4,
    PAPI_MPX_POST_CAM = 0x8,
    PAPI_MPX_POST_TAM = 0x10,
    PAPI_MPX_POST_DCAM = 0x20,
    PAPI_MPX_POST_DIRA = 0x40
} PAPI_MPX_postprocessing_method;

/* variables for sampling attributes. */
typedef struct mpx_attr {
    pid_t mainpid; // pid of the main thread
    pid_t childpid; // pid of the sampling thread 
    pthread_t tsample; // pthread id of sampling thread
    PAPI_MPX_status status; // status of sampling thread
    int *EventSet; // EventSet for multiplexing
    int Event; // Event added for multiplexing
    int sampling_interval; // Interval for between sampling point(nanosecond)
    long long *values; // values to store the final multiplexing result
    PAPI_MPX_data_flag data_flag;
    PAPI_MPX_postprocessing_method method;
} PAPI_MPX_attr; 
```

# PAPI
See more about [PAPI](README_PAPI.md)
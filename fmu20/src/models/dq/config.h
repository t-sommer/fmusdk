#ifndef config_h
#define config_h

// define class name and unique id
#define MODEL_IDENTIFIER dq
#define MODEL_GUID "{8c4e810f-3df3-4a00-8276-176fa3c9f000}"

// define model size
#define NUMBER_OF_STATES 1
#define NUMBER_OF_EVENT_INDICATORS 0

#define GET_REAL
#define SET_REAL
#define EVENT_UPDATE

typedef enum {
    vr_x, vr_der_x, vr_k
} ValueReference;

typedef struct {

    double x;
    double der_x;
    double k;

} ModelData;

#endif /* config_h */

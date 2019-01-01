#ifndef config_h
#define config_h

// define class name and unique id
#define MODEL_IDENTIFIER bouncingBall
#define MODEL_GUID "{8c4e810f-3df3-4a00-8276-176fa3c9f003}"

// define model size
#define NUMBER_OF_STATES 2
#define NUMBER_OF_EVENT_INDICATORS 1

#define GET_REAL
#define SET_REAL
#define EVENT_UPDATE

typedef enum {
    vr_h, vr_der_h, vr_v, vr_der_v, vr_g, vr_e
} ValueReference;

typedef struct {
    
    double h;
    double v;
    double g;
    double e;
    
} ModelData;

#endif /* config_h */

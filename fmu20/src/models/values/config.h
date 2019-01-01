#ifndef config_h
#define config_h

// define class name and unique id
#define MODEL_IDENTIFIER values
#define MODEL_GUID "{8c4e810f-3df3-4a00-8276-176fa3c9f004}"

// define model size
#define NUMBER_OF_STATES 0
#define NUMBER_OF_EVENT_INDICATORS 0

#define GET_REAL
#define GET_INTEGER
#define GET_BOOLEAN

#define SET_REAL
#define SET_INTEGER
#define SET_BOOLEAN

#define EVENT_UPDATE

typedef enum {
    vr_real_in,
    vr_real_out,
    vr_int_in,
    vr_int_out,
    vr_bool_in,
    vr_bool_out,
    vr_string,
} ValueReference;

typedef struct {
    double      r;
    int         i;
    int         b;
    const char *s;
} ModelData;

#endif /* config_h */

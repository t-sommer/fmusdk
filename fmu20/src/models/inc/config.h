#ifndef config_h
#define config_h

// define class name and unique id
#define MODEL_IDENTIFIER inc
#define MODEL_GUID "{8c4e810f-3df3-4a00-8276-176fa3c9f008}"

// define model size
#define NUMBER_OF_STATES 0
#define NUMBER_OF_EVENT_INDICATORS 0

#define GET_INTEGER
#define EVENT_UPDATE

typedef enum {
    vr_counter
} ValueReference;

typedef struct {
    
    double counter;
    
} ModelData;

#endif /* config_h */

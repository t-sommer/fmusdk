#ifndef model_h
#define model_h

#include "stddef.h"  // for size_t

#include "config.h"

// categories of logging supported by model.
// Value is the index in logCategories of a ModelInstance.
#define LOG_ALL       0
#define LOG_ERROR     1
#define LOG_FMI_CALL  2
#define LOG_EVENT     3

#define NUMBER_OF_CATEGORIES 4

#if FMI_VERSION == 1

#define not_modelError (modelInstantiated|modelInitialized|modelTerminated)

typedef enum {
    modelInstantiated = 1<<0,
    modelInitialized  = 1<<1,
    modelTerminated   = 1<<2,
    modelError        = 1<<3
} ModelState;

#else

typedef enum {
    modelStartAndEnd        = 1<<0,
    modelInstantiated       = 1<<1,
    modelInitializationMode = 1<<2,

    // ME states
    modelEventMode          = 1<<3,
    modelContinuousTimeMode = 1<<4,
    
    // CS states
    modelStepComplete       = 1<<5,
    modelStepInProgress     = 1<<6,
    modelStepFailed         = 1<<7,
    modelStepCanceled       = 1<<8,

    modelTerminated         = 1<<9,
    modelError              = 1<<10,
    modelFatal              = 1<<11,
} ModelState;

#endif

typedef enum {
    OK,
    Warning,
    Discard,
    Error,
    Fatal,
    Pending
} Status;


#if FMI_VERSION > 1
typedef int boolean;
#else
typedef char boolean;
#endif

#define FALSE 0
#define TRUE  1

typedef struct {
    
    double time;
    const char *instanceName;
    int type;
    const char *GUID;

    // callback functions
    void  (*logger)(void *, const char *, int, const char *, const char *, ...);
    void* (*allocateMemory)(size_t, size_t);
    void  (*freeMemory)(void *);
    void  (*stepFinished)(void *, int);
    
    boolean loggingOn;
    boolean logCategories[NUMBER_OF_CATEGORIES];

    void *componentEnvironment;
    ModelState state;
    
    // event info
    int newDiscreteStatesNeeded;
    int terminateSimulation;
    int nominalsOfContinuousStatesChanged;
    int valuesOfContinuousStatesChanged;
    int nextEventTimeDefined;
    double nextEventTime;
    
    boolean isDirtyValues;
    boolean isNewEventIteration;
    
    ModelData *modelData;
    void *solverData;
    
} ModelInstance;
    
void setStartValues(ModelInstance *comp);
void calculateValues(ModelInstance *comp);
    
Status getReal(ModelInstance* comp, ValueReference vr, double *value);
Status getInteger(ModelInstance* comp, ValueReference vr, int *value);
Status getBoolean(ModelInstance* comp, ValueReference vr, boolean *value);

Status setReal(ModelInstance* comp, ValueReference vr, double value);
Status setInteger(ModelInstance* comp, ValueReference vr, int value);
Status setBoolean(ModelInstance* comp, ValueReference vr, boolean value);

void getContinuousStates(ModelInstance *comp, double x[], size_t nx);
void setContinuousStates(ModelInstance *comp, const double x[], size_t nx);
void getDerivatives(ModelInstance *comp, double dx[], size_t nx);
void getEventIndicators(ModelInstance *comp, double z[], size_t nz);
void eventUpdate(ModelInstance *comp);

#endif  /* model_h */

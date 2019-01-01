#include <math.h>  // for fabs()

#include "config.h"

// include fmu header files, typedefs and macros
#include "model.h"


// shorthand to access the variables
#define M(v) (comp->modelData->v)


void setStartValues(ModelInstance *comp) {
    M(counter) = 1;
    
    // TODO: move this to initialize()?
    comp->nextEventTime        = 1;
    comp->nextEventTimeDefined = TRUE;
}

void calculateValues(ModelInstance *comp) {
    // do nothing
}

Status getInteger(ModelInstance* comp, ValueReference vr, int *value) {
    switch (vr) {
        case vr_counter: *value = M(counter); return OK;
        default: return Error;
    }
}

void eventUpdate(ModelInstance *comp) {
    
    if (comp->nextEventTimeDefined && comp->nextEventTime == comp->time) {
        M(counter)++;
        comp->nextEventTime += 1;
    }
    
    comp->valuesOfContinuousStatesChanged   = FALSE;
    comp->nominalsOfContinuousStatesChanged = FALSE;
    comp->terminateSimulation               = FALSE;
    comp->nextEventTimeDefined              = TRUE;
}

#include "config.h"

// include fmu header files, typedefs and macros
#include "model.h"


// shorthand to access the variables
#define M(v) (comp->modelData->v)


void setStartValues(ModelInstance *comp) {
    M(r)     = 0;
    M(i)     = 0;
    M(b)     = 0;
    M(s)     = "a";
}

void calculateValues(ModelInstance *comp) {
    // do nothing
}

Status getReal(ModelInstance* comp, ValueReference vr, double *value) {
    calculateValues(comp);
    switch (vr) {
        case vr_real_in:    *value = M(r);     return OK;
        case vr_real_out:   *value = M(r);     return OK;
        default: return Error;
    }
}

Status getInteger(ModelInstance* comp, ValueReference vr, int *value) {
    calculateValues(comp);
    switch (vr) {
        case vr_int_in:  *value = M(i); return OK;
        case vr_int_out: *value = M(i); return OK;
        default: return Error;
    }
}

Status getBoolean(ModelInstance* comp, ValueReference vr, boolean *value) {
    calculateValues(comp);
    switch (vr) {
        case vr_bool_in:  *value = M(b); return OK;
        case vr_bool_out: *value = M(b); return OK;
        default: return Error;
    }
}

Status setReal(ModelInstance* comp, ValueReference vr, double value) {
    switch (vr) {
        case vr_real_in: M(r) = value; return OK;
        default: return Error;
    }
}

Status setInteger(ModelInstance* comp, ValueReference vr, int value) {
    switch (vr) {
        case vr_int_in: M(i) = value; return OK;
        default: return Error;
    }
}

Status setBoolean(ModelInstance* comp, ValueReference vr, boolean value) {
    switch (vr) {
        case vr_bool_in: M(b) = value; return OK;
        default: return Error;
    }
}

void eventUpdate(ModelInstance *comp) {
    comp->valuesOfContinuousStatesChanged   = FALSE;
    comp->nominalsOfContinuousStatesChanged = FALSE;
    comp->terminateSimulation               = FALSE;
    comp->nextEventTimeDefined              = FALSE;
}

#include "config.h"

// include fmu header files, typedefs and macros
#include "model.h"


// shorthand to access the variables
#define M(v) (comp->modelData->v)


void setStartValues(ModelInstance *comp) {
    M(x) = 1;
    M(k) = 1;
}

void calculateValues(ModelInstance *comp) {
    M(der_x) = -M(k) * M(x);
}

Status getReal(ModelInstance* comp, ValueReference vr, double *value) {
    calculateValues(comp);
    switch (vr) {
        case vr_x    : *value = M(x);     return OK;
        case vr_der_x: *value = M(der_x); return OK;
        case vr_k    : *value = M(k);     return OK;
        default: return Error;
    }
}

Status setReal(ModelInstance* comp, ValueReference vr, double value) {
    switch (vr) {
        case vr_x: M(x) = value; return OK;
        case vr_k: M(k) = value; return OK;
        default: return Error;
    }
}

void getContinuousStates(ModelInstance *comp, double x[], size_t nx) {
    x[0] = M(x);
}

void setContinuousStates(ModelInstance *comp, const double x[], size_t nx) {
    M(x) = x[0];
}

void getDerivatives(ModelInstance *comp, double dx[], size_t nx) {
    calculateValues(comp);
    dx[0] = M(der_x);
}

void eventUpdate(ModelInstance *comp) {
    comp->valuesOfContinuousStatesChanged   = FALSE;
    comp->nominalsOfContinuousStatesChanged = FALSE;
    comp->terminateSimulation               = FALSE;
    comp->nextEventTimeDefined              = FALSE;
}

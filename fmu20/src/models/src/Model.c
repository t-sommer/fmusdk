#include "model.h"


// default implementations
#if NUMBER_OF_EVENT_INDICATORS < 1
void getEventIndicators(ModelInstance *comp, double z[], size_t nz) {
    // do nothing
}
#endif

#ifndef GET_REAL
Status getReal(ModelInstance* comp, ValueReference vr, double *value) {
    return Error;
}
#endif

#ifndef GET_INTEGER
Status getInteger(ModelInstance* comp, ValueReference vr, int *value) {
    return Error;
}
#endif

#ifndef GET_BOOLEAN
Status getBoolean(ModelInstance* comp, ValueReference vr, boolean *value) {
    return Error;
}
#endif

#if NUMBER_OF_STATES < 1
void getContinuousStates(ModelInstance *comp, double x[], size_t nx) {}
void setContinuousStates(ModelInstance *comp, const double x[], size_t nx) {}
void getDerivatives(ModelInstance *comp, double dx[], size_t nx) {}
#endif

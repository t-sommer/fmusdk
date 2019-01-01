/* ---------------------------------------------------------------------------*
 * Sample implementation of an FMU - a bouncing ball.
 * This demonstrates the use of state events and reinit of states.
 * Equations:
 *  der(h) = v;
 *  der(v) = -g;
 *  when h<0 then v := -e * v;
 *  where
 *    h      height [m], used as state, start = 1
 *    v      velocity of ball [m/s], used as state
 *    der(h) velocity of ball [m/s]
 *    der(v) acceleration of ball [m/s2]
 *    g      acceleration of gravity [m/s2], a parameter, start = 9.81
 *    e      a dimensionless parameter, start = 0.7
 *
 * Copyright QTronic GmbH. All rights reserved.
 * ---------------------------------------------------------------------------*/

#include <math.h>  // for fabs()

#include "config.h"

// include fmu header files, typedefs and macros
#include "model.h"


// shorthand to access the variables
#define M(v) (comp->modelData->v)


void setStartValues(ModelInstance *comp) {
    M(h) =  1;
    M(v) =  0;
    M(g) = -9.81;
    M(e) =  0.7;
}

void calculateValues(ModelInstance *comp) {
    // do nothing
}

Status getReal(ModelInstance* comp, ValueReference vr, double *value) {
    switch (vr) {
        case vr_h     : *value = M(h); return OK;
        case vr_der_h : *value = M(v); return OK;
        case vr_v     : *value = M(v); return OK;
        case vr_der_v : *value = M(g); return OK;
        case vr_g     : *value = M(g); return OK;
        case vr_e     : *value = M(e); return OK;
        default: return Error;
    }
}

Status setReal(ModelInstance* comp, ValueReference vr, double value) {
    switch (vr) {
        case vr_h: M(h) = value; return OK;
        case vr_v: M(v) = value; return OK;
        case vr_g: M(g) = value; return OK;
        case vr_e: M(e) = value; return OK;
        default: return Error;
    }
}

void eventUpdate(ModelInstance *comp) {

    if (M(h) <= 0) {

        M(h) = 0;
        M(v) = fabs(M(v) * M(e));

        if (M(v) <= 1e-3) {
            // stop bouncing
            M(v) = 0;
            M(g) = 0;
        }

        comp->valuesOfContinuousStatesChanged = TRUE;
    }

    comp->nominalsOfContinuousStatesChanged = FALSE;
    comp->terminateSimulation  = FALSE;
    comp->nextEventTimeDefined = FALSE;
}

void getContinuousStates(ModelInstance *comp, double x[], size_t nx) {
    x[0] = M(h);
    x[1] = M(v);
}

void setContinuousStates(ModelInstance *comp, const double x[], size_t nx) {
    M(h) = x[0];
    M(v) = x[1];
}

void getDerivatives(ModelInstance *comp, double dx[], size_t nx) {
    dx[0] = M(v);
    dx[1] = M(g);
}

void getEventIndicators(ModelInstance *comp, double z[], size_t nz) {
    z[0] = M(h);
}

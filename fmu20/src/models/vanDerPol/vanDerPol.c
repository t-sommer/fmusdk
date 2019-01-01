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

#include "config.h"

// include fmu header files, typedefs and macros
#include "model.h"


// shorthand to access the variables
#define M(v) (comp->modelData->v)


void setStartValues(ModelInstance *comp) {
    M(x0) = 2;
    M(x1) = 0;
    M(mu) = 1;
}

void calculateValues(ModelInstance *comp) {
    M(der_x0) = M(x1);
    M(der_x1) = M(mu) * ((1.0 - M(x0) * M(x0)) * M(x1)) - M(x0);
}

Status getReal(ModelInstance* comp, ValueReference vr, double *value) {
    calculateValues(comp);
    switch (vr) {
        case vr_x0     : *value = M(x0);     return OK;
        case vr_der_x0 : *value = M(der_x0); return OK;
        case vr_x1     : *value = M(x1);     return OK;
        case vr_der_x1 : *value = M(der_x1); return OK;
        case vr_mu     : *value = M(mu);     return OK;
        default: return Error;
    }
}

Status setReal(ModelInstance* comp, ValueReference vr, double value) {
    switch (vr) {
        case vr_x0: M(x0 = value); return OK;
        case vr_x1: M(x1 = value); return OK;
        case vr_mu: M(mu = value); return OK;
        default: return Error;
    }
}

void getContinuousStates(ModelInstance *comp, double x[], size_t nx) {
    x[0] = M(x0);
    x[1] = M(x1);
}

void setContinuousStates(ModelInstance *comp, const double x[], size_t nx) {
    M(x0) = x[0];
    M(x1) = x[1];
}

void getDerivatives(ModelInstance *comp, double dx[], size_t nx) {
    calculateValues(comp);
    dx[0] = M(der_x0);
    dx[1] = M(der_x1);
}

void eventUpdate(ModelInstance *comp) {
    comp->valuesOfContinuousStatesChanged   = FALSE;
    comp->nominalsOfContinuousStatesChanged = FALSE;
    comp->terminateSimulation               = FALSE;
    comp->nextEventTimeDefined              = FALSE;
}

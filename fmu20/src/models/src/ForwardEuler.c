#include "solver.h"
#include <stdlib.h>


typedef struct {
    int nx;
    int nz;
    double *x;
    double *dx;
    double *z;
    double *prez;
} SolverData;

void *solver_create(ModelInstance *comp) {
    SolverData *s = (SolverData *)comp->allocateMemory(NULL, 1, sizeof(SolverData));
    s->nx   = NUMBER_OF_STATES;
    s->nz   = NUMBER_OF_EVENT_INDICATORS;
    s->x    = (double *)calloc(s->nx, sizeof(double));
    s->dx   = (double *)calloc(s->nx, sizeof(double));
    s->z    = (double *)calloc(s->nz, sizeof(double));
    s->prez = (double *)calloc(s->nz, sizeof(double));
    return s;
}

void solver_step(ModelInstance *comp, double t, double tNext, double *tRet, int *stateEvent) {
    
    SolverData *s = (SolverData *)comp->solverData;

    // step size
    const double h = tNext - t;
    
    double *temp;
    
    // set continuous states
    getContinuousStates(comp, s->x, s->nx);
    
    // get derivatives
    getDerivatives(comp, s->dx, s->nx);
    
    // forward Euler step
    for (int i = 0; i < s->nx; i++) {
        s->x[i] += h * s->dx[i];
    }
    
    // tNext has been reached
    *tRet = tNext;
    
    // set continuous states
    setContinuousStates(comp, s->x, s->nx);
    
    // get event indicators
    getEventIndicators(comp, s->z, s->nz);
    
    *stateEvent = FALSE;
    
    // check for zero-crossing
    for (int i = 0; i < s->nz; i++) {
        *stateEvent |= (s->prez[i] * s->z[i]) <= 0;
    }
    
    // remember the current event indicators
    temp = s->z;
    s->z = s->prez;
    s->prez = temp;
}

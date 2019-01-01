#include "config.h"
#include "slave.h"
#include "solver.h"


Status doStep(ModelInstance *comp, double t, double tNext) {
    
    int stateEvent = 0;
    int timeEvent  = 0;
    
    comp->time = t;
    
    while (comp->time < tNext) {
        
        if (comp->nextEventTimeDefined && comp->nextEventTime < tNext) {
            solver_step(comp, comp->time, comp->nextEventTime, &comp->time, &stateEvent);
        } else {
            solver_step(comp, comp->time, tNext, &comp->time, &stateEvent);
        }
        
        // check for time event
        if (comp->nextEventTimeDefined && (comp->time - comp->nextEventTime > -1e-5)) {
            timeEvent = TRUE;
        }
        
        if (stateEvent || timeEvent) {
            eventUpdate(comp);
            timeEvent  = 0;
            stateEvent = 0;
        }
        
        // terminate simulation, if requested by the model in the previous step
        if (comp->terminateSimulation) {
#if FMI_VERSION > 1
            comp->state = modelStepFailed;
#endif
            return Discard; // enforce termination of the simulation loop
        }
        
    }
    
    return OK;
}

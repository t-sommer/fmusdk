#include "config.h"
#include <cfloat> // for DBL_MAX
#include "CVodeSolver.h"


void Slave::doStep(double currentCommunicationPoint, double communicationStepSize, double noSetFMUStatePriorToCurrentPoint) {
    
    bool stateEvent = false;
    
    bool valuesOfContinuousStatesChanged = false;
    
    setTime(currentCommunicationPoint);
    
    auto time = currentCommunicationPoint;
    
    double tNext = currentCommunicationPoint + communicationStepSize;
    
    while (time < tNext) {

        m_solver->step(currentCommunicationPoint, tNext, &time, &stateEvent);
        
        bool timeEvent = m_nextEventTime <= time;
        
        setTime(time);
        
        bool inputEvent = false; // TODO: detect input events
        
        bool stepEvent = false;
        
        // check for step event (e.g. dynamic state selection)
        completedIntegratorStep(&stepEvent);
        
        if (inputEvent || timeEvent || stateEvent || stepEvent) {
            
            update(&valuesOfContinuousStatesChanged, &m_nextEventTime);
            
            // reset solver
            m_solver->reset(time);
        }
        
    }
}

void Slave::initialize() {
    // instantiate the solver
    m_solver = new SOLVER(*this);
    bool dummy;
    update(&dummy, &m_nextEventTime);
}

void Slave::exitInitializationMode() {
    m_solver->reset(getTime());
}

#ifndef Slave_h
#define Slave_h

#include "Model.h"

class Solver;

class Slave : public Model {
    
public:
    virtual ~Slave() {}
    virtual void initialize();
    virtual void exitInitializationMode();
    virtual void doStep(double currentCommunicationPoint, double communicationStepSize, double noSetFMUStatePriorToCurrentPoint);
    
private:
    Solver *m_solver;
    double m_nextEventTime;
    
};

#endif /* Slave_h */

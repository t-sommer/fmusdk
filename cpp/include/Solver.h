#ifndef Solver_h
#define Solver_h

class Solver {
    
public:
    virtual void step(double t, double tNext, double *tRet, bool *stateEvent) = 0;
    virtual void reset(double time) = 0;
    
};

#endif /* Solver_h */

#ifndef CVode_hpp
#define CVode_hpp

#include "Solver.h"
#include "Model.h"
#include "cvode/cvode.h"

class CVodeSolver : public Solver {
    
public:
    explicit CVodeSolver(Model &fmu);
    void step(double t, double tNext, double *tRet, bool *stateEvent) override;
    void reset(double time) override;

private:
    Model &m_fmu;
    void *m_cvode_mem;
    N_Vector m_x;
    N_Vector m_abstol;
    
    // RHS function
    static int f(realtype t, N_Vector y, N_Vector ydot, void *user_data);
    
    // Root function
    static int g(realtype t, N_Vector y, realtype *gout, void *user_data);

};

#endif /* CVode_hpp */

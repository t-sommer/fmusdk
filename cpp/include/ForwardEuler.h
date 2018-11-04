#ifndef ForwardEuler_h
#define ForwardEuler_h

#include <algorithm>  // for std::swap

#include "Solver.h"
#include "Model.h"
#include "Slave.h"

class ForwardEuler : public Solver {
    
public:
    explicit ForwardEuler(Model& fmu) : m_fmu(fmu) {
        
        // states and derivatives
        m_nx = fmu.getNumberOfContinuousStates();

        m_x = new double[m_nx];
        m_fmu.getContinuousStates(m_x);

        m_dx = new double[m_nx];
        
        // event indicators
        m_nz = fmu.getNumberOfEventIndicators();

        m_z = new double[m_nz];

        m_prez = new double[m_nz];
        fmu.getEventIndicators(m_prez);
    }
    
    ~ForwardEuler() {
        // clean up
        delete m_x;
        delete m_dx;
        delete m_z;
        delete m_prez;
    }
    
    void step(double t, double tNext, double *tRet, bool *stateEvent) override {

        // step size
        const double h = tNext - t;
        
        // get derivatives
        m_fmu.getDerivatives(m_dx);
        
        // forward Euler step
        for (int i = 0; i < m_nx; i++) {
            m_x[i] += h * m_dx[i];
        }
        
        // tNext has been reached
        *tRet = tNext;
        
        // set continuous states
        m_fmu.setContinuousStates(m_x);
        
        // get event indicators
        m_fmu.getEventIndicators(m_z);
        
        *stateEvent = false;
        
        // check for zero-crossing
        for (int i = 0; i < m_nz; i++) {
            *stateEvent |= (m_prez[i] * m_z[i]) <= 0;
        }
        
        // remember the current event indicators
        std::swap(m_z, m_prez);
    }
    
    void reset(double time) override {
        // get states and event indicators
        m_fmu.getContinuousStates(m_x);
        m_fmu.getEventIndicators(m_prez);
    }
    
private:
    Model&     m_fmu;
    int       m_nx;  // number of states
    double    *m_x;  // continuous states
    double   *m_dx;  // derivatives of continuous states
    int       m_nz;  // number of event indicators
    double    *m_z;  // event indicators
    double *m_prez;  // previous event indicators
};

#endif /* ForwardEuler_h */

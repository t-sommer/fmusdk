#ifndef VanDerPol_h
#define VanDerPol_h

#include "Slave.h"


class VanDerPol : public Slave {
    
private:
    // value references
    enum ValueReference {
        vr_x0 = 1,
        vr_dx0,
        vr_x1,
        vr_dx1,
        vr_mu
    };
    
    // variables
    double m_x0;
    double m_dx0;
    double m_x1;
    double m_dx1;
    double m_mu;

    struct FMUState {
        State state;
        double x0, dx0, x1, dx1, mu;
    };
    
public:    
    int getNumberOfContinuousStates() override { return 2; }
    int getNumberOfEventIndicators() override { return 0; }

    std::vector<double> getDouble(int vr) override {
        
        calculateDerivatives();
        
        switch (vr) {
            case vr_x0:  return { m_x0 };
            case vr_dx0: return { m_dx0 };
            case vr_x1:  return { m_x1 };
            case vr_dx1: return { m_dx1 };
            case vr_mu:  return { m_mu };
            default:     return Slave::getDouble(vr);
        }
        
    }
    
    void setDouble(int vr, const double* value, int* index) override {
        
        switch (vr) {
            case vr_x0:  m_x0  = value[*index++]; break;
            case vr_dx0: m_dx0 = value[*index++]; break;
            case vr_x1:  m_x1  = value[*index++]; break;
            case vr_dx1: m_dx1 = value[*index++]; break;
            case vr_mu:  m_mu  = value[*index++]; break;
            default: Slave::setDouble(vr, value, index);
        }
        
    }
    
    void getContinuousStates(double x[]) override {
        x[0] = m_x0;
        x[1] = m_x1;
    }
    
    void setContinuousStates(const double x[]) override {
        m_x0 = x[0];
        m_x1 = x[1];
    }
    
    void getDerivatives(double dx[]) override {
        
        calculateDerivatives();
        
        dx[0] = m_dx0;
        dx[1] = m_dx1;
    }
    
    void initialize() override {
                
        // set start values
        m_x0  = 2;
        m_x1  = 0;
        m_mu  = 1;

        calculateDerivatives();
        
        Slave::initialize();
    }
    
private:
    
    void calculateDerivatives() {
        m_dx0 = m_x1;
        m_dx1 = m_mu * ((1.0 - m_x0 * m_x0) * m_x1) - m_x0;
    }

};

#endif /* VanDerPol_h */

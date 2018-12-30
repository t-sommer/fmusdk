#ifndef BouncingBall_h
#define BouncingBall_h

#include "Slave.h"
#include <cmath>  // for std::abs

class BouncingBall : public Slave {
    
private:
    // value references
    enum ValueReference {
        vr_h = 1,
        vr_v,
        vr_g,
        vr_e,
    };
    
    // variables
    double m_h;
    double m_v;
    double m_g;
    double m_e;
    
    struct FMUState {
        State state;
        double h, v, g, e;
    };
    
public:    
    int getNumberOfContinuousStates() override { return 2; }
    int getNumberOfEventIndicators() override { return 1; }

    std::vector<double> getDouble(int vr) override {
        
        switch (vr) {
            case vr_h: return { m_h };
            case vr_v: return { m_v };
            case vr_g: return { m_g };
            case vr_e: return { m_e };
            default:   return Slave::getDouble(vr);
        }
        
    }
    
    void setDouble(int vr, const double* value, int* index) override {
        
        switch (vr) {
            case vr_h: m_h = value[*index++]; break;
            case vr_v: m_v = value[*index++]; break;
            case vr_g: m_g = value[*index++]; break;
            case vr_e: m_e = value[*index++]; break;
            default: Slave::setDouble(vr, value, index);
        }
        
    }
    
    void getContinuousStates(double x[]) override {
        x[0] = m_h;
        x[1] = m_v;
    }
    
    void setContinuousStates(const double x[]) override {
        m_h = x[0];
        m_v = x[1];
    }
    
    void getDerivatives(double dx[]) override {
        dx[0] = m_v;
        dx[1] = m_g;
    }
    
    void getEventIndicators(double z[]) override {
        z[0] = m_h;
    }
    
    void initialize() override {
                
        // set start values
        m_h =  1;
        m_v =  0;
        m_g = -9.81;
        m_e =  0.7;
        
        Slave::initialize();
    }
    
    void update(bool *continuousStatesChanged, double *nextEventTime) override {
        
        if (m_h <= 0) {
            
            m_h = 0;
            m_v = std::abs(m_v * m_e);
            
            if (m_v <= 1e-3) {
                // stop bouncing
                m_v = 0;
                m_g = 0;
            }
            
            *continuousStatesChanged = true;
        }
    }
    
    void* getFMUState() override {
        return new FMUState { m_state, m_h, m_v, m_g, m_e };
    }
    
    void setFMUState(void *fmuState) override {
        auto s = reinterpret_cast<FMUState*>(fmuState);
        m_state = s->state;
        m_h = s->h;
        m_v = s->v;
        m_g = s->g;
        m_e = s->e;
    }
    
    size_t getSerializedFMUStateSize(void* fmuState) override {
        return sizeof(FMUState);
    }
    
    void serializeFMUState(void *fmuState, char *memory, size_t size) override {
        
        if (size < sizeof(FMUState)) {
            error("Not enough memory to serialize FMU state");
        }
        
        memcpy(memory, fmuState, sizeof(FMUState));
    }
    
    void deserializeFMUState(const char *memory, size_t size, void** fmuState) override {
        
        if (size < sizeof(FMUState)) {
            error("Not enough memory to de-serialize FMU state");
        }
        
        auto state = reinterpret_cast<FMUState*>(*fmuState);
        
        if (state == nullptr) {
            state = new FMUState();
        }
        
        auto serializedState = reinterpret_cast<const FMUState*>(memory);
        
        *state = *serializedState;
        
        *fmuState = state;
    }
    
    virtual void freeFMUState(void *fmuState) override {
        free(fmuState);
    }

};

#endif /* BouncingBall_h */

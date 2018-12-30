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
    double h;
    double v;
    double g;
    double e;
    
    struct FMUState {
        double h, v, g, e;
    };
    
public:    
    int getNumberOfContinuousStates() override { return 2; }
    int getNumberOfEventIndicators() override { return 1; }

    std::vector<double> getDouble(int vr) override {
        
        switch (vr) {
            case vr_h: return { h };
            case vr_v: return { v };
            case vr_g: return { g };
            case vr_e: return { e };
            default:   return Slave::getDouble(vr);
        }
        
    }
    
    void setDouble(int vr, const double* value, int* index) override {
        
        switch (vr) {
            case vr_h: h = value[*index++]; break;
            case vr_v: v = value[*index++]; break;
            case vr_g: g = value[*index++]; break;
            case vr_e: e = value[*index++]; break;
            default: Slave::setDouble(vr, value, index);
        }
        
    }
    
    void getContinuousStates(double x[]) override {
        x[0] = h;
        x[1] = v;
    }
    
    void setContinuousStates(const double x[]) override {
        h = x[0];
        v = x[1];
    }
    
    void getDerivatives(double dx[]) override {
        dx[0] = v;
        dx[1] = g;
    }
    
    void getEventIndicators(double z[]) override {
        z[0] = h;
    }
    
    void initialize() override {
                
        // set start values
        h =  1;
        v =  0;
        g = -9.81;
        e =  0.7;
        
        Slave::initialize();
    }
    
    void update(bool *continuousStatesChanged, double *nextEventTime) override {
        
        if (h <= 0) {
            
            h = 0;
            v = std::abs(v * e);
            
            if (v <= 1e-3) {
                // stop bouncing
                v = 0;
                g = 0;
            }
            
            *continuousStatesChanged = true;
        }
    }
    
    void* getFMUState() override {
        return new FMUState { h, v, g, e };
    }
    
    void setFMUState(void *fmuState) override {
        auto s = reinterpret_cast<FMUState*>(fmuState);
        h = s->h;
        v = s->v;
        g = s->g;
        e = s->e;
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

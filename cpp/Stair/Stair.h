#ifndef Stair_h
#define Stair_h

#include <cmath>   // for std::fmod, std::fmax
#include <string>  // for printf
#include "Slave.h"


class Stair : public Slave {
    
private:
    // value references
    enum ValueReference {
        vr_y = 1
    };
    
    // variables
    double y;
    
public:
    int getNumberOfContinuousStates() override { return 0; }
    int getNumberOfEventIndicators() override { return 0; }

    void getDouble(int vr, double* value, int* index) override {
        
        switch (vr) {
            case vr_y:
                value[(*index)++] = y; break;
            default:
                return Slave::getDouble(vr, value, index);
        }
        
    }
    
    void setDouble(int vr, const double* value, int* index) override {

        switch (vr) {
            case vr_y: y = value[*index++]; break;
            default: break;
        }
        
    }
    
    void initialize() override {
        
        // set start values
        y = 0;
        
        Slave::initialize();
    }
    
    void update(bool *continuousStatesChanged, double *nextEventTime) override {
        int ticks = std::floor(getTime() / 0.1);
        *nextEventTime = (ticks + 1) * 0.1;
        y = std::fmax(0, std::floor(getTime() / 0.1));
    }
    
};

#endif /* Stair_h */

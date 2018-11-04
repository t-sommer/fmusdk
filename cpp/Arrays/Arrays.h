#ifndef Stair_h
#define Stair_h

#include <cmath>   // for std::fmod, std::fmax
#include <string>  // for printf
#include "Slave.h"


class Arrays : public Slave {
    
private:
    // value references
    enum ValueReference {
        vr_y
    };
    
    // variables
    std::vector<double> y;
    
public:
    int getNumberOfContinuousStates() override { return 0; }
    int getNumberOfEventIndicators() override { return 0; }

    std::vector<double> getDouble(int vr) override {
        
        switch (vr) {
            case vr_y:
                return { y };
            default:
                return Slave::getDouble(vr);
        }
        
    }
    
    void initialize() override {
        
        // set start values
        y = { 1, 2, 3 };
        
        Slave::initialize();
    }
    
};

#endif /* Stair_h */

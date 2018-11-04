#ifndef Stair_h
#define Stair_h

#include <cmath>   // for std::fmod, std::fmax
#include <string>  // for printf
#include "Slave.h"


class Stair : public Slave {
    
private:
    // value references
    enum ValueReference {
        vr_double_in,
        vr_double_out,
        vr_float32_in,
        vr_float32_out,
        vr_int8_in,
        vr_int8_out,
        vr_uint8_in,
        vr_uint8_out,
        vr_int16_in,
        vr_int16_out,
        vr_uint16_in,
        vr_uint16_out,
        vr_int32_in,
        vr_int32_out,
        vr_uint32_in,
        vr_uint32_out,
        vr_int64_in,
        vr_int64_out,
        vr_uint64_in,
        vr_uint64_out,
        vr_bool_in,
        vr_bool_out,
    };
    
    // variables
    double m_double;
    float m_float32;
    int8_t m_int8;
    uint8_t m_uint8;
    int16_t m_int16;
    uint16_t m_uint16;
    int32_t m_int32;
    uint32_t m_uint32;
    int64_t m_int64;
    uint64_t m_uint64;
    bool m_bool;
    
public:
    int getNumberOfContinuousStates() override { return 0; }
    int getNumberOfEventIndicators() override { return 0; }

    std::vector<double> getDouble(int vr) override {
        
        switch (vr) {
            case vr_double_in:
            case vr_double_out:
                return { m_double };
            default:
                return Model::getDouble(vr);
        }
        
    }
    
    std::vector<float> getFloat(int vr) override {
        
        switch (vr) {
            case vr_float32_in:
            case vr_float32_out:
                return { m_float32 };
            default:
                return Model::getFloat(vr);
        }
        
    }
    
    std::vector<int8_t> getInt8(int vr) override {
        
        switch (vr) {
            case vr_int8_in:
            case vr_int8_out:
                return { m_int8 };
            default:
                return Model::getInt8(vr);
        }
        
    }
    
    std::vector<uint8_t> getUInt8(int vr) override {
        
        switch (vr) {
            case vr_uint8_in:
            case vr_uint8_out:
                return { m_uint8 };
            default:
                return Model::getUInt8(vr);
        }
        
    }
    
    std::vector<int16_t> getInt16(int vr) override {
        
        switch (vr) {
            case vr_int16_in:
            case vr_int16_out:
                return { m_int16 };
            default:
                return Model::getInt16(vr);
        }
        
    }
    
    std::vector<uint16_t> getUInt16(int vr) override {
        
        switch (vr) {
            case vr_uint16_in:
            case vr_uint16_out:
                return { m_uint16 };
            default:
                return Model::getUInt16(vr);
        }
        
    }
    
    std::vector<int32_t> getInt32(int vr) override {
        
        switch (vr) {
            case vr_int32_in:
            case vr_int32_out:
                return { m_int32 };
            default:
                return Model::getInt32(vr);
        }
        
    }
    
    std::vector<uint32_t> getUInt32(int vr) override {
        
        switch (vr) {
            case vr_uint32_in:
            case vr_uint32_out:
                return { m_uint32 };
            default:
                return Model::getUInt32(vr);
        }
        
    }
    
    std::vector<int64_t> getInt64(int vr) override {
        
        switch (vr) {
            case vr_int64_in:
            case vr_int64_out:
                return { m_int64 };
            default:
                return Model::getInt64(vr);
        }
        
    }
    
    std::vector<uint64_t> getUInt64(int vr) override {
        
        switch (vr) {
            case vr_uint64_in:
            case vr_uint64_out:
                return { m_uint64 };
            default:
                return Model::getUInt64(vr);
        }
        
    }
    
    std::vector<bool> getBool(int vr) override {
        
        switch (vr) {
            case vr_bool_in:
            case vr_bool_out:
                return { m_bool };
            default:
                return Model::getBool(vr);
        }
        
    }
    
    void setDouble(int vr, const double* value, int* index) override {

        switch (vr) {
            case vr_double_in:
            case vr_double_out:
                m_double = value[*index++]; break;
                break;
            default:
                Model::getDouble(vr);
        }
        
    }

    void setInt32(int vr, int32_t value) override {
        
        switch (vr) {
            case vr_int32_in:
            case vr_int32_out:
                m_int32 = value;
                break;
            default:
                Model::getInt32(vr);
        }
        
    }
    
    void setUInt64(int vr, uint64_t value) override {
        
        switch (vr) {
            case vr_uint64_in:
            case vr_uint64_out:
                m_uint64 = value;
                break;
            default:
                Model::setUInt64(vr, value);
        }
        
    }
    
    void setBool(int vr, bool value) override {
        
        switch (vr) {
            case vr_bool_in:
            case vr_bool_out:
                m_bool = value;
                break;
            default:
                Model::getBool(vr);
        }
        
    }
    
    void initialize() override {
        
        // set start values
        m_double = 0;
        m_int32 = 0;
        m_uint64 = 0;
        m_bool = false;
        
        Slave::initialize();
    }
    
};

#endif /* Stair_h */

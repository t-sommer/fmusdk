#ifndef Stair_h
#define Stair_h

#include <cmath>   // for std::fmod, std::fmax
#include <string>  // for printf
#include "Slave.h"


class Stair : public Slave {
    
private:
    // value references
    enum ValueReference {
        vr_double_in = 1,
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
    boolean m_bool;
    
public:
    int getNumberOfContinuousStates() override { return 0; }
    int getNumberOfEventIndicators() override { return 0; }

    void getDouble(int vr, double* value, int* index) override {
        
        switch (vr) {
            case vr_double_in:
            case vr_double_out:
                value[(*index)++] = m_double; break;
            default:
                Slave::getDouble(vr, value, index);
        }
        
    }
    
    void getFloat(int vr, float* value, int* index) override {
        
        switch (vr) {
            case vr_float32_in:
            case vr_float32_out:
                value[(*index)++] = m_float32; break;
            default:
                Slave::getFloat(vr, value, index);
        }
        
    }
    
    void getInt8(int vr, int8_t* value, int* index) override {
        
        switch (vr) {
            case vr_int8_in:
            case vr_int8_out:
                value[(*index)++] = m_int8; break;
            default:
                Slave::getInt8(vr, value, index);
        }
        
    }
    
    void getUInt8(int vr, uint8_t* value, int* index) override {
        
        switch (vr) {
            case vr_uint8_in:
            case vr_uint8_out:
                value[(*index)++] = m_uint8; break;
            default:
                Slave::getUInt8(vr, value, index);
        }
        
    }
    
    void getInt16(int vr, int16_t* value, int* index) override {
        
        switch (vr) {
            case vr_int16_in:
            case vr_int16_out:
                value[(*index)++] = m_int16; break;
            default:
                Slave::getInt16(vr, value, index);
        }
        
    }
    
    void getUInt16(int vr, uint16_t* value, int* index) override {
        
        switch (vr) {
            case vr_uint16_in:
            case vr_uint16_out:
                value[(*index)++] = m_uint16; break;
            default:
                Slave::getUInt16(vr, value, index);
        }
        
    }
    
    void getInt32(int vr, int32_t* value, int* index) override {
        
        switch (vr) {
            case vr_int32_in:
            case vr_int32_out:
                value[(*index)++] = m_int32; break;
            default:
                Slave::getInt32(vr, value, index);
        }
        
    }
    
    void getUInt32(int vr, uint32_t* value, int* index) override {
        
        switch (vr) {
            case vr_uint32_in:
            case vr_uint32_out:
                value[(*index)++] = m_uint32; break;
            default:
                Slave::getUInt32(vr, value, index);
        }
        
    }

    void getInt64(int vr, int64_t* value, int* index) override {
        
        switch (vr) {
            case vr_int64_in:
            case vr_int64_out:
                value[(*index)++] = m_int64; break;
            default:
                Slave::getInt64(vr, value, index);
        }
        
    }
    
    void getUInt64(int vr, uint64_t* value, int* index) override {
        
        switch (vr) {
            case vr_uint64_in:
            case vr_uint64_out:
                value[(*index)++] = m_uint64; break;
            default:
                Slave::getUInt64(vr, value, index);
        }
        
    }

    void getBoolean(int vr, boolean* value, int* index) override {
        
        switch (vr) {
            case vr_bool_in:
            case vr_bool_out:
                value[(*index)++] = m_bool; break;
            default:
                Slave::getBoolean(vr, value, index);
        }
        
    }
    
    void setDouble(int vr, const double* value, int* index) override {

        switch (vr) {
            case vr_double_in:
            case vr_double_out:
                m_double = value[*index++];
                break;
            default:
                Model::setDouble(vr, value, index);
        }
        
    }

    void setInt32(int vr, const int32_t* value, int* index) override {
        
        switch (vr) {
            case vr_int32_in:
            case vr_int32_out:
                m_int32 = value[*index++];
                break;
            default:
                Model::setInt32(vr, value, index);
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
    
    void setBoolean(int vr, const boolean value[], int* index) override {
        
        switch (vr) {
            case vr_bool_in:
            case vr_bool_out:
                m_bool = value[*index++];
                break;
            default:
                Model::setBoolean(vr, value, index);
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

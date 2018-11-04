#ifndef Model_h
#define Model_h

#include "stdint.h"
#include <string>
#include <vector>


class Model {
    
public:
    virtual ~Model() {}
    
    virtual void enterInitializationMode() {}
    virtual void exitInitializationMode() {}

    virtual int getNumberOfContinuousStates() = 0;
    virtual int getNumberOfEventIndicators() = 0;
    
    virtual std::vector<double> getDouble(int vr);
    virtual std::vector<float> getFloat(int vr);
    virtual std::vector<int8_t> getInt8(int vr);
    virtual std::vector<uint8_t> getUInt8(int vr);
    virtual std::vector<int16_t> getInt16(int vr);
    virtual std::vector<uint16_t> getUInt16(int vr);
    virtual std::vector<int32_t> getInt32(int vr);
    virtual std::vector<uint32_t> getUInt32(int vr);
    virtual std::vector<int64_t> getInt64(int vr);
    virtual std::vector<uint64_t> getUInt64(int vr);
    virtual std::vector<bool> getBool(int vr);
    virtual std::vector<const std::string*> getString(int vr);
    virtual void setDouble(int vr, const double *value, int* index);
    //virtual void setDouble(int vr, const std::vector<double>& value);
    virtual void setInt32(int vr, int32_t value);
    virtual void setUInt64(int vr, uint64_t value);
    virtual void setBool(int vr, bool value);
    virtual void setString(int vr, const std::string& value);
    virtual void getContinuousStates(double x[]) {}
    virtual void setContinuousStates(const double x[]) {}
    virtual void getDerivatives(double dx[]) {}
    virtual void getEventIndicators(double z[]) {}
    virtual void update(bool *valuesOfContinuousStatesChanged, double *nextEventTime) {}
    virtual void setTime(double time) { m_time = time; }
    virtual double getTime() { return m_time; }
    
    virtual void completedIntegratorStep(bool *enterEventMode) {};
    
    virtual void enterContinuousTimeMode() {}
    virtual void enterEventMode() {}
    
protected:
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message) __attribute__((noreturn));
    
private:
    double m_time;

public:
    void *m_logger;
    
};

#endif /* Model_h */
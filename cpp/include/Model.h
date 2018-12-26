#ifndef Model_h
#define Model_h

#include "stdint.h"
#include <string>
#include <vector>


#if __STDC_VERSION__ >= 201112L
#define NORETURN _Noreturn
#define NORETURNATTR
#elif defined(__cplusplus) && __cplusplus >= 201103L
#if (defined(__GNUC__) && __GNUC__ >= 5) || \
    (defined(__GNUC__) && defined(__GNUC_MINOR__) && __GNUC__ == 4 && __GNUC_MINOR__ >= 8)
#define NORETURN [[noreturn]]
#define NORETURNATTR
#elif (defined(__GNUC__) && __GNUC__ >= 3) || \
      (defined(__GNUC__) && defined(__GNUC_MINOR__) && __GNUC__ == 2 && __GNUC_MINOR__ >= 8)
#define NORETURN
#define NORETURNATTR __attribute__((noreturn))
#elif defined(__GNUC__)
#define NORETURN
#define NORETURNATTR
#else
#define NORETURN [[noreturn]]
#define NORETURNATTR
#endif
#elif defined(__clang__)
/* Encapsulated for Clang since GCC fails to process __has_attribute */
#if __has_attribute(noreturn)
#define NORETURN
#define NORETURNATTR __attribute__((noreturn))
#else
#define NORETURN
#define NORETURNATTR
#endif
#elif (defined(__GNUC__) && __GNUC__ >= 3) || \
      (defined(__GNUC__) && defined(__GNUC_MINOR__) && __GNUC__ == 2 && __GNUC_MINOR__ >= 8) || \
      (defined(__SUNPRO_C) && __SUNPRO_C >= 0x5110)
#define NORETURN
#define NORETURNATTR __attribute__((noreturn))
#elif (defined(_MSC_VER) && _MSC_VER >= 1200) || \
       defined(__BORLANDC__)
#define NORETURN __declspec(noreturn)
#define NORETURNATTR
#else
#define NORETURN
#define NORETURNATTR
#endif


enum State : int;


class Model {
    
public:

    State m_state;
    int m_fmiType;

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
    virtual void setDouble(int vr, const double value[], int* index);
    virtual void setInt32(int vr, const int32_t value[], int* index);
    virtual void setUInt64(int vr, uint64_t value);
    virtual void setBool(int vr, const bool value[], int* index);
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
    void assertState(State state);
    void info(const std::string& message);
    void warning(const std::string& message);
	NORETURN void error(const std::string& message) NORETURNATTR;

private:
    double m_time;
    std::string m_instanceName;

public:
    void *m_logger;
    
};

#endif /* Model_h */

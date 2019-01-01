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

typedef int boolean;


class Model {
    
public:

    State m_state;
    int m_fmiType;

    virtual ~Model() {}
    
    virtual void enterInitializationMode() {}
    virtual void exitInitializationMode() {}

    virtual int getNumberOfContinuousStates() = 0;
    virtual int getNumberOfEventIndicators() = 0;
    
    virtual void getDouble(int vr, double value[], int* index);
    virtual void getFloat(int vr, float value[], int* index);
    virtual void getInt8(int vr, int8_t value[], int* index);
    virtual void getUInt8(int vr, uint8_t value[], int* index);
    virtual void getInt16(int vr, int16_t value[], int* index);
    virtual void getUInt16(int vr, uint16_t value[], int* index);
    virtual void getInt32(int vr, int32_t value[], int* index);
    virtual void getUInt32(int vr, uint32_t value[], int* index);
    virtual void getInt64(int vr, int64_t value[], int* index);
    virtual void getUInt64(int vr, uint64_t value[], int* index);
    virtual void getBoolean(int vr, boolean value[], int* index);
    virtual void getString(int vr, const char* value[], int* index);
    virtual void setDouble(int vr, const double value[], int* index);
    virtual void setInt32(int vr, const int32_t value[], int* index);
    virtual void setUInt64(int vr, uint64_t value);
    virtual void setBoolean(int vr, const boolean value[], int* index);
    virtual void setString(int vr, const std::string& value);
    virtual void getContinuousStates(double x[]) {}
    virtual void setContinuousStates(const double x[]) {}
    virtual void getDerivatives(double dx[]) {}
    virtual void getEventIndicators(double z[]) {}
    virtual void update(bool *valuesOfContinuousStatesChanged, double *nextEventTime) {}
    virtual void setTime(double time) { m_time = time; }
    virtual double getTime() { return m_time; }
    
    virtual void* getFMUState();
    virtual void setFMUState(void *fmuState);
    virtual void freeFMUState(void *fmuState);
    virtual size_t getSerializedFMUStateSize(void* fmuState);
    virtual void serializeFMUState(void *fmuState, char *memory, size_t size);
    virtual void deserializeFMUState(const char *memory, size_t size, void** fmuState);

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

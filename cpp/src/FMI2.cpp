#include "config.h"
#include "fmi2Functions.h"
#include <cfloat> // for DBL_MAX

#define INSTANCE (static_cast<Model*>(c))

void Model::info(const std::string &message) {
    auto logger = (fmi2CallbackLogger)(this->m_logger);
    logger(this, "instance", fmi2OK, "info", message.c_str());
}

void Model::warning(const std::string &message) {
    auto logger = (fmi2CallbackLogger)(this->m_logger);
    logger(this, "instance", fmi2Warning, "warning", message.c_str());
}

void Model::error(const std::string &message) {
    auto logger = (fmi2CallbackLogger)(this->m_logger);
    logger(this, "instance", fmi2Error, "error", message.c_str());
    throw std::exception();
}

template<class T>
fmi2Status get_v(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, T value[], size_t nValues, std::vector<T> (Model::*getter)(int)) {
    
    try {
        int i = 0;
        
        for (int j = 0; j < nvr; j++) {
            
            auto values = (INSTANCE->*getter)(vr[j]);
            
            for (int k = 0; k < values.size(); k++) {
                
                if (i >= nValues) {
                    // TODO: log error message
                    return fmi2Error;
                }
                
                value[i++] = values[k];
            }
        }
    } catch (std::exception const &e) {
        return fmi2Error;
    }
    
    return fmi2OK;
}

template<class T>
fmi2Status set_v(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const T value[], size_t nValues, void (Model::*setter)(int, const double*, int*)) {
    
    try {
        int i = 0;
        
        for (int j = 0; j < nvr; j++) {
            
            (INSTANCE->*setter)(vr[j], value, &i);
            
            if (i >= nValues) {
                // TODO: log error message
                return fmi2Error;
            }
            
        }
    } catch (std::exception const &e) {
        return fmi2Error;
    }
    
    return fmi2OK;
}

extern "C" {
    
fmi2Status setString(fmi2Component comp, fmi2ValueReference vr, fmi2String value) {
    return fmi2Error;
}

// ---------------------------------------------------------------------------
// FMI functions
// ---------------------------------------------------------------------------
fmi2Component fmi2Instantiate(fmi2String instanceName, fmi2Type fmuType, fmi2String fmuGUID,
                              fmi2String fmuResourceLocation, const fmi2CallbackFunctions *functions,
                              fmi2Boolean visible, fmi2Boolean loggingOn) {
    auto component = new MODEL_NAME();
    component->m_logger = reinterpret_cast<void *>(functions->logger);
    component->initialize();
    return component;
}

fmi2Status fmi2SetupExperiment(fmi2Component c, fmi2Boolean toleranceDefined, fmi2Real tolerance,
                               fmi2Real startTime, fmi2Boolean stopTimeDefined, fmi2Real stopTime) {
    return fmi2OK;
}

fmi2Status fmi2EnterInitializationMode(fmi2Component c) {
    return fmi2OK;
}

fmi2Status fmi2ExitInitializationMode(fmi2Component c) {
    INSTANCE->exitInitializationMode();
    return fmi2OK;
}

fmi2Status fmi2Terminate(fmi2Component c) {
    return fmi2OK;
}

fmi2Status fmi2Reset(fmi2Component c) {
    return fmi2OK;
}

void fmi2FreeInstance(fmi2Component c) {
    
}

// ---------------------------------------------------------------------------
// FMI functions: class methods not depending of a specific model instance
// ---------------------------------------------------------------------------

const char* fmi2GetVersion() {
    return fmi2Version;
}

const char* fmi2GetTypesPlatform() {
    return fmi2TypesPlatform;
}

// ---------------------------------------------------------------------------
// FMI functions: logging control, setters and getters for Real, Integer,
// Boolean, String
// ---------------------------------------------------------------------------

fmi2Status fmi2SetDebugLogging(fmi2Component c, fmi2Boolean loggingOn, size_t nCategories, const fmi2String categories[]) {
    return fmi2OK;
}

fmi2Status fmi2GetReal (fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Real value[]) {
    return get_v(c, vr, nvr, value, nvr, &Model::getDouble);
}

fmi2Status fmi2GetInteger(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Integer value[]) {
    return fmi2Error;
}

fmi2Status fmi2GetBoolean(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Boolean value[]) {
    return fmi2Error;
}

fmi2Status fmi2GetString (fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2String value[]) {
    return fmi2Error;
}

fmi2Status fmi2SetReal (fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Real value[]) {
    return set_v(c, vr, nvr, value, nvr, &Model::setDouble);
}

fmi2Status fmi2SetInteger(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Integer value[]) {
    return fmi2Error;
}

fmi2Status fmi2SetBoolean(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Boolean value[]) {
    return fmi2Error;
}

fmi2Status fmi2SetString (fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2String value[]) {
    return fmi2Error;
}

fmi2Status fmi2GetFMUstate (fmi2Component c, fmi2FMUstate* FMUstate) {
    return fmi2Error;
}
fmi2Status fmi2SetFMUstate (fmi2Component c, fmi2FMUstate FMUstate) {
    return fmi2Error;
}
fmi2Status fmi2FreeFMUstate(fmi2Component c, fmi2FMUstate* FMUstate) {
    return fmi2Error;
}
fmi2Status fmi2SerializedFMUstateSize(fmi2Component c, fmi2FMUstate FMUstate, size_t *size) {
    return fmi2Error;
}
fmi2Status fmi2SerializeFMUstate (fmi2Component c, fmi2FMUstate FMUstate, fmi2Byte serializedState[], size_t size) {
    return fmi2Error;
}
fmi2Status fmi2DeSerializeFMUstate (fmi2Component c, const fmi2Byte serializedState[], size_t size,
                                    fmi2FMUstate* FMUstate) {
    return fmi2Error;
}

fmi2Status fmi2GetDirectionalDerivative(fmi2Component c, const fmi2ValueReference vUnknown_ref[], size_t nUnknown,
                                        const fmi2ValueReference vKnown_ref[] , size_t nKnown,
                                        const fmi2Real dvKnown[], fmi2Real dvUnknown[]) {
    return fmi2Error;
}

// ---------------------------------------------------------------------------
// Functions for FMI for Co-Simulation
// ---------------------------------------------------------------------------

/* Simulating the slave */
fmi2Status fmi2SetRealInputDerivatives(fmi2Component c, const fmi2ValueReference vr[], size_t nvr,
                                       const fmi2Integer order[], const fmi2Real value[]) {
    return fmi2Error;
}

fmi2Status fmi2GetRealOutputDerivatives(fmi2Component c, const fmi2ValueReference vr[], size_t nvr,
                                        const fmi2Integer order[], fmi2Real value[]) {
    return fmi2Error;
}

fmi2Status fmi2CancelStep(fmi2Component c) {
    return fmi2Error;
}

fmi2Status fmi2DoStep(fmi2Component c, fmi2Real currentCommunicationPoint, fmi2Real communicationStepSize, fmi2Boolean noSetFMUStatePriorToCurrentPoint) {
    
    auto *slave = static_cast<Slave*>(c);
    
    slave->doStep(currentCommunicationPoint, communicationStepSize, noSetFMUStatePriorToCurrentPoint);
    
    return fmi2OK;
}

fmi2Status fmi2GetStatus(fmi2Component c, const fmi2StatusKind s, fmi2Status *value) {
    return fmi2Error;
}

fmi2Status fmi2GetRealStatus(fmi2Component c, const fmi2StatusKind s, fmi2Real *value) {
    return fmi2Error;
}

fmi2Status fmi2GetIntegerStatus(fmi2Component c, const fmi2StatusKind s, fmi2Integer *value) {
    return fmi2Error;
}

fmi2Status fmi2GetBooleanStatus(fmi2Component c, const fmi2StatusKind s, fmi2Boolean *value) {
    return fmi2Error;
}

fmi2Status fmi2GetStringStatus(fmi2Component c, const fmi2StatusKind s, fmi2String *value) {
    return fmi2Error;
}

// ---------------------------------------------------------------------------
// Functions for FMI2 for Model Exchange
// ---------------------------------------------------------------------------

/* Enter and exit the different modes */
fmi2Status fmi2EnterEventMode(fmi2Component c) {
    return fmi2OK;
}

fmi2Status fmi2NewDiscreteStates(fmi2Component c, fmi2EventInfo *eventInfo) {
    
    bool valuesOfContinuousStatesChanged = false;
    double nextEventTime = DBL_MAX;
    
    INSTANCE->update(&valuesOfContinuousStatesChanged, &nextEventTime);
    
    eventInfo->newDiscreteStatesNeeded = fmi2False;
    eventInfo->valuesOfContinuousStatesChanged = fmi2True;
    eventInfo->nextEventTimeDefined = eventInfo->nextEventTime < DBL_MAX;
    eventInfo->nextEventTime = nextEventTime;
    
    return fmi2OK;
}

fmi2Status fmi2EnterContinuousTimeMode(fmi2Component c) {
    return fmi2OK;
}

fmi2Status fmi2CompletedIntegratorStep(fmi2Component c, fmi2Boolean noSetFMUStatePriorToCurrentPoint, fmi2Boolean *enterEventMode, fmi2Boolean *terminateSimulation) {
    return fmi2OK;
}

/* Providing independent variables and re-initialization of caching */
fmi2Status fmi2SetTime(fmi2Component c, fmi2Real time) {
    INSTANCE->setTime(time);
    return fmi2OK;
}

fmi2Status fmi2SetContinuousStates(fmi2Component c, const fmi2Real x[], size_t nx) {
    INSTANCE->setContinuousStates(x);
    return fmi2OK;
}

/* Evaluation of the model equations */
fmi2Status fmi2GetDerivatives(fmi2Component c, fmi2Real derivatives[], size_t nx) {
    INSTANCE->getDerivatives(derivatives);
    return fmi2OK;
}

fmi2Status fmi2GetEventIndicators(fmi2Component c, fmi2Real eventIndicators[], size_t ni) {
    INSTANCE->getEventIndicators(eventIndicators);
    return fmi2OK;
}

fmi2Status fmi2GetContinuousStates(fmi2Component c, fmi2Real states[], size_t nx) {
    INSTANCE->getContinuousStates(states);
    return fmi2OK;
}

fmi2Status fmi2GetNominalsOfContinuousStates(fmi2Component c, fmi2Real x_nominal[], size_t nx) {
    return fmi2Error;
}
    
} // extern "C"

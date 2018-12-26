#include "config.h"
#include "fmi2Functions.h"
#include <cfloat> // for DBL_MAX
#include <set>

#define INSTANCE (static_cast<Model*>(c))

enum State : int {
    StartAndEnd        = 1<<0,
    Instantiated       = 1<<1,
    InitializationMode = 1<<2,
    
    // model exchange
    EventMode          = 1<<3,
    ContinuousTimeMode = 1<<4,
    
    // co-simulation
    StepComplete       = 1<<5,
    StepInProgress     = 1<<6,
    StepFailed         = 1<<7,
    StepCanceled       = 1<<8,
    
    Terminated         = 1<<9,
    Error              = 1<<10,
    Fatal              = 1<<11,
};


void Model::assertState(State state) {
    if (m_state != state) {
        error("Illegal calling sequence");
    }
}

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
fmi2Status set_v(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const T value[], void (Model::*setter)(int, const T*, int*)) {
    
    try {
        int i = 0;
        
        for (int j = 0; j < nvr; j++) {
            
            (INSTANCE->*setter)(vr[j], value, &i);
            
            if (i >= nvr) {
                // TODO: log error message
                return fmi2Error;
            }
            
        }
    } catch (std::exception const &e) {
        return fmi2Error;
    }
    
    return fmi2OK;
}

bool isLegalState(fmi2Component c, void* function) {
    
    int mask = 0;
    
    // ---------------------------------------------------------------------------
    // Function calls allowed state masks for both Model-exchange and Co-simulation
    // ---------------------------------------------------------------------------
    
    if (false) {}
    else if (function == fmi2GetTypesPlatform ||
             function == fmi2GetVersion ||
             function == fmi2SetDebugLogging)             { mask = StartAndEnd
                                                            | Instantiated
                                                            | InitializationMode
                                                            | EventMode
                                                            | ContinuousTimeMode
                                                            | StepComplete
                                                            | StepInProgress
                                                            | StepFailed
                                                            | StepCanceled
                                                            | Terminated
                                                            | Error; }
    else if (function == fmi2Instantiate)             { mask = StartAndEnd; }
    else if (function == fmi2EnterInitializationMode) { mask = Instantiated; }
    else if (function == fmi2ExitInitializationMode)  { mask = InitializationMode; }
    else if (function == fmi2SetupExperiment)         { mask = Instantiated; }
    else if (function == fmi2GetReal    ||
             function == fmi2GetInteger ||
             function == fmi2GetBoolean ||
             function == fmi2GetString)               { mask = Instantiated
                                                            | InitializationMode
                                                            | EventMode
                                                            | ContinuousTimeMode
                                                            | StepComplete; }
    else if (function == fmi2SetReal)               { mask = Instantiated
                                                            | InitializationMode
                                                            | EventMode
                                                            | ContinuousTimeMode
                                                            | StepComplete; }
    else if (function == fmi2SetInteger ||
             function == fmi2SetBoolean ||
             function == fmi2SetString)               { mask = Instantiated
                                                            | InitializationMode
                                                            | EventMode
                                                            | StepComplete; }
    else if (function == fmi2GetFMUstate ||
             function == fmi2SetFMUstate ||
             function == fmi2FreeFMUstate ||
             function == fmi2SerializedFMUstateSize ||
             function == fmi2SerializeFMUstate ||
             function == fmi2DeSerializeFMUstate ||
             function == fmi2FreeInstance ||
             function == fmi2Reset)                    { mask = Instantiated
                                                            | InitializationMode
                                                            | EventMode
                                                            | ContinuousTimeMode
                                                            | StepComplete
                                                            | StepFailed
                                                            | StepCanceled
                                                            | Terminated
                                                            | Error;
    }
    else if (function == fmi2Terminate)                 { mask = EventMode
                                                            | ContinuousTimeMode
                                                            | StepComplete
                                                            | StepFailed; }
    else if (function == fmi2GetDirectionalDerivative) { mask = InitializationMode
                                                            | EventMode
                                                            | ContinuousTimeMode
                                                            | StepComplete
                                                            | StepFailed
                                                            | StepCanceled
                                                            | Terminated
                                                            | Error; }
    
    // ---------------------------------------------------------------------------
    // Function calls allowed state masks for Model-exchange
    // ---------------------------------------------------------------------------
    else if (function == fmi2EnterEventMode)                { mask = EventMode |
                                                                     ContinuousTimeMode; }
    else if (function == fmi2NewDiscreteStates)             { mask = EventMode; }
    else if (function == fmi2EnterContinuousTimeMode)       { mask = EventMode; }
    else if (function == fmi2CompletedIntegratorStep)       { mask = ContinuousTimeMode; }
    else if (function == fmi2SetTime)                       { mask = EventMode |
                                                                     ContinuousTimeMode; }
    else if (function == fmi2SetContinuousStates)           { mask = ContinuousTimeMode; }
    else if (function == fmi2GetEventIndicators ||
             function == fmi2GetContinuousStates)           { mask = InitializationMode |
                                                                     EventMode |
                                                                     ContinuousTimeMode |
                                                                     Terminated |
                                                                     Error; }
    else if (function == fmi2GetDerivatives)                { mask = EventMode
                                                                   | ContinuousTimeMode
                                                                   | Terminated
                                                                   | Error; }
    else if (function == fmi2GetNominalsOfContinuousStates) { mask = InitializationMode
                                                                   | EventMode
                                                                   | ContinuousTimeMode
                                                                   | Terminated
                                                                   | Error; }
    
    // ---------------------------------------------------------------------------
    // Function calls allowed state masks for Co-simulation
    // ---------------------------------------------------------------------------
    
    else if (function == fmi2SetRealInputDerivatives) { mask = Instantiated
                                                            | InitializationMode
                                                            | StepComplete; }
    else if (function == fmi2GetRealOutputDerivatives) { mask = StepComplete
                                                            | StepFailed
                                                            | StepCanceled
                                                            | Terminated
                                                            | Error; }
    else if (function == fmi2DoStep) {                   mask = StepComplete; }
    else if (function == fmi2CancelStep) {               mask = StepInProgress; }
    else if (function == fmi2GetStatus ||
             function == fmi2GetRealStatus ||
             function == fmi2GetIntegerStatus ||
             function == fmi2GetIntegerStatus ||
             function == fmi2GetStringStatus) {          mask = StepComplete
                                                            | StepInProgress
                                                            | StepFailed
                                                            | Terminated; }
    
    if (!(INSTANCE->m_state & mask)) {
        auto logger = (fmi2CallbackLogger)(INSTANCE->m_logger);
        logger(INSTANCE, "instance", fmi2Error, "error", "Illegal calling sequence");
        return false;
    }
    
    return true;
}

#define ASSERT_STATE(FUNC) if (!isLegalState(c, (void *)FUNC)) { return fmi2Error; }


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
    
    component->m_fmiType = fmuType;
    component->m_logger = reinterpret_cast<void *>(functions->logger);
    component->initialize();
    component->m_state = Instantiated;
    
    return component;
}

fmi2Status fmi2SetupExperiment(fmi2Component c, fmi2Boolean toleranceDefined, fmi2Real tolerance,
                               fmi2Real startTime, fmi2Boolean stopTimeDefined, fmi2Real stopTime) {
    ASSERT_STATE(fmi2SetupExperiment)
    return fmi2OK;
}

fmi2Status fmi2EnterInitializationMode(fmi2Component c) {
    ASSERT_STATE(fmi2EnterInitializationMode)
    INSTANCE->m_state = InitializationMode;
    return fmi2OK;
}

fmi2Status fmi2ExitInitializationMode(fmi2Component c) {
    ASSERT_STATE(fmi2ExitInitializationMode)
    INSTANCE->exitInitializationMode();
    INSTANCE->m_state = INSTANCE->m_fmiType == fmi2ModelExchange ? EventMode : StepComplete;
    return fmi2OK;
}

fmi2Status fmi2Terminate(fmi2Component c) {
    ASSERT_STATE(fmi2Terminate)
    return fmi2OK;
}

fmi2Status fmi2Reset(fmi2Component c) {
    ASSERT_STATE(fmi2Reset)
    return fmi2OK;
}

void fmi2FreeInstance(fmi2Component c) {
    // TODO: ASSERT_STATE(fmi2FreeInstance)
    INSTANCE->m_state = StartAndEnd;
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
    ASSERT_STATE(fmi2SetDebugLogging)
    return fmi2OK;
}

fmi2Status fmi2GetReal(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Real value[]) {
    ASSERT_STATE(fmi2GetReal)
    return get_v(c, vr, nvr, value, nvr, &Model::getDouble);
}

fmi2Status fmi2GetInteger(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Integer value[]) {
    ASSERT_STATE(fmi2GetInteger)
    return get_v(c, vr, nvr, value, nvr, &Model::getInt32);
}

fmi2Status fmi2GetBoolean(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Boolean value[]) {
    
    ASSERT_STATE(fmi2GetBoolean)
    
    try {
        for (int i = 0; i < nvr; i++) {
            auto values = INSTANCE->getBool(vr[i]);
            value[i] = values[0];
        }
    } catch (std::exception const &e) {
        return fmi2Error;
    }

    return fmi2OK;
}

fmi2Status fmi2GetString(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2String value[]) {
    
    ASSERT_STATE(fmi2GetString)
    
    try {
        for (int i = 0; i < nvr; i++) {
            auto values = INSTANCE->getString(vr[i]);
            value[i] = values[0]->c_str();
        }
    } catch (std::exception const &e) {
        return fmi2Error;
    }

    return fmi2OK;
}

fmi2Status fmi2SetReal(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Real value[]) {
    ASSERT_STATE(fmi2SetReal)
    return set_v(c, vr, nvr, value, &Model::setDouble);
}

fmi2Status fmi2SetInteger(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Integer value[]) {
    ASSERT_STATE(fmi2SetInteger)
    return set_v(c, vr, nvr, value, &Model::setInt32);
}

fmi2Status fmi2SetBoolean(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Boolean value[]) {
    
    ASSERT_STATE(fmi2SetBoolean)
    
    try {
        for (int j = 0; j < nvr; j++) {
            bool bool_value = value[j] != fmi2False;
            int i = 0;
            INSTANCE->setBool(vr[j], &bool_value, &i);
        }
    } catch (std::exception const &e) {
        return fmi2Error;
    }
    
    return fmi2OK;
}

fmi2Status fmi2SetString(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2String value[]) {
    ASSERT_STATE(fmi2SetString)
    return fmi2Error;
}

fmi2Status fmi2GetFMUstate(fmi2Component c, fmi2FMUstate* FMUstate) {
    ASSERT_STATE(fmi2GetFMUstate)
    return fmi2Error;

}

fmi2Status fmi2SetFMUstate(fmi2Component c, fmi2FMUstate FMUstate) {
    ASSERT_STATE(fmi2SetFMUstate)
    return fmi2Error;
}

fmi2Status fmi2FreeFMUstate(fmi2Component c, fmi2FMUstate* FMUstate) {
    ASSERT_STATE(fmi2FreeFMUstate)
    return fmi2Error;
}

fmi2Status fmi2SerializedFMUstateSize(fmi2Component c, fmi2FMUstate FMUstate, size_t *size) {
    ASSERT_STATE(fmi2SerializedFMUstateSize)
    return fmi2Error;
}

fmi2Status fmi2SerializeFMUstate(fmi2Component c, fmi2FMUstate FMUstate, fmi2Byte serializedState[], size_t size) {
    ASSERT_STATE(fmi2SerializeFMUstate)
    return fmi2Error;
}

fmi2Status fmi2DeSerializeFMUstate(fmi2Component c, const fmi2Byte serializedState[], size_t size,
                                    fmi2FMUstate* FMUstate) {
    ASSERT_STATE(fmi2DeSerializeFMUstate)
    return fmi2Error;
}

fmi2Status fmi2GetDirectionalDerivative(fmi2Component c, const fmi2ValueReference vUnknown_ref[], size_t nUnknown,
                                        const fmi2ValueReference vKnown_ref[] , size_t nKnown,
                                        const fmi2Real dvKnown[], fmi2Real dvUnknown[]) {
    ASSERT_STATE(fmi2GetDirectionalDerivative)
    return fmi2Error;
}

// ---------------------------------------------------------------------------
// Functions for FMI for Co-Simulation
// ---------------------------------------------------------------------------

/* Simulating the slave */
fmi2Status fmi2SetRealInputDerivatives(fmi2Component c, const fmi2ValueReference vr[], size_t nvr,
                                       const fmi2Integer order[], const fmi2Real value[]) {
    ASSERT_STATE(fmi2SetRealInputDerivatives)
    return fmi2Error;
}

fmi2Status fmi2GetRealOutputDerivatives(fmi2Component c, const fmi2ValueReference vr[], size_t nvr,
                                        const fmi2Integer order[], fmi2Real value[]) {
    ASSERT_STATE(fmi2GetRealOutputDerivatives)
    return fmi2Error;
}

fmi2Status fmi2CancelStep(fmi2Component c) {
    ASSERT_STATE(fmi2CancelStep)
    return fmi2Error;
}

fmi2Status fmi2DoStep(fmi2Component c, fmi2Real currentCommunicationPoint, fmi2Real communicationStepSize, fmi2Boolean noSetFMUStatePriorToCurrentPoint) {
    ASSERT_STATE(fmi2DoStep)
    auto *slave = static_cast<Slave*>(c);
    slave->doStep(currentCommunicationPoint, communicationStepSize, noSetFMUStatePriorToCurrentPoint);
    return fmi2OK;
}

fmi2Status fmi2GetStatus(fmi2Component c, const fmi2StatusKind s, fmi2Status *value) {
    ASSERT_STATE(fmi2GetStatus)
    return fmi2Error;
}

fmi2Status fmi2GetRealStatus(fmi2Component c, const fmi2StatusKind s, fmi2Real *value) {
    ASSERT_STATE(fmi2GetRealStatus)
    return fmi2Error;
}

fmi2Status fmi2GetIntegerStatus(fmi2Component c, const fmi2StatusKind s, fmi2Integer *value) {
    ASSERT_STATE(fmi2GetIntegerStatus)
    return fmi2Error;
}

fmi2Status fmi2GetBooleanStatus(fmi2Component c, const fmi2StatusKind s, fmi2Boolean *value) {
    ASSERT_STATE(fmi2GetBooleanStatus)
    return fmi2Error;
}

fmi2Status fmi2GetStringStatus(fmi2Component c, const fmi2StatusKind s, fmi2String *value) {
    ASSERT_STATE(fmi2GetStringStatus)
    return fmi2Error;
}

// ---------------------------------------------------------------------------
// Functions for FMI2 for Model Exchange
// ---------------------------------------------------------------------------

/* Enter and exit the different modes */
fmi2Status fmi2EnterEventMode(fmi2Component c) {
    ASSERT_STATE(fmi2EnterEventMode)
    INSTANCE->m_state = EventMode;
    return fmi2OK;
}

fmi2Status fmi2NewDiscreteStates(fmi2Component c, fmi2EventInfo *eventInfo) {
    
    ASSERT_STATE(fmi2NewDiscreteStates)
    
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
    ASSERT_STATE(fmi2EnterContinuousTimeMode)
    INSTANCE->m_state = ContinuousTimeMode;
    return fmi2OK;
}

fmi2Status fmi2CompletedIntegratorStep(fmi2Component c, fmi2Boolean noSetFMUStatePriorToCurrentPoint, fmi2Boolean *enterEventMode, fmi2Boolean *terminateSimulation) {
    ASSERT_STATE(fmi2CompletedIntegratorStep)
    return fmi2OK;
}

/* Providing independent variables and re-initialization of caching */
fmi2Status fmi2SetTime(fmi2Component c, fmi2Real time) {
    ASSERT_STATE(fmi2SetTime)
    INSTANCE->setTime(time);
    return fmi2OK;
}

fmi2Status fmi2SetContinuousStates(fmi2Component c, const fmi2Real x[], size_t nx) {
    ASSERT_STATE(fmi2SetContinuousStates)
    INSTANCE->setContinuousStates(x);
    return fmi2OK;
}

/* Evaluation of the model equations */
fmi2Status fmi2GetDerivatives(fmi2Component c, fmi2Real derivatives[], size_t nx) {
    ASSERT_STATE(fmi2GetDerivatives)
    INSTANCE->getDerivatives(derivatives);
    return fmi2OK;
}

fmi2Status fmi2GetEventIndicators(fmi2Component c, fmi2Real eventIndicators[], size_t ni) {
    ASSERT_STATE(fmi2GetEventIndicators)
    INSTANCE->getEventIndicators(eventIndicators);
    return fmi2OK;
}

fmi2Status fmi2GetContinuousStates(fmi2Component c, fmi2Real states[], size_t nx) {
    ASSERT_STATE(fmi2GetContinuousStates)
    INSTANCE->getContinuousStates(states);
    return fmi2OK;
}

fmi2Status fmi2GetNominalsOfContinuousStates(fmi2Component c, fmi2Real x_nominal[], size_t nx) {
    ASSERT_STATE(fmi2GetNominalsOfContinuousStates)
    return fmi2Error;
}
    
} // extern "C"

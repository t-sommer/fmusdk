#include "config.h"
#include "fmi3Functions.h"
#include <cfloat> // for DBL_MAX

#define INSTANCE (static_cast<Model*>(c))

void Model::info(const std::string &message) {
    auto logger = (fmi3CallbackLogger)(this->m_logger);
    logger(this, "instance", fmi3OK, "info", message.c_str());
}

void Model::warning(const std::string &message) {
    auto logger = (fmi3CallbackLogger)(this->m_logger);
    logger(this, "instance", fmi3Warning, "warning", message.c_str());
}

void Model::error(const std::string &message) {
    auto logger = (fmi3CallbackLogger)(this->m_logger);
    logger(this, "instance", fmi3Error, "error", message.c_str());
    throw std::exception();
}

template<class T>
fmi3Status get(fmi3Component c, const fmi3ValueReference vr[], size_t nvr, T value[], size_t nValues, T (Model::*getter)(int)) {
    
    try {
        for (int i = 0; i < nvr; i++) {
            value[i] = (INSTANCE->*getter)(vr[i]);
        }
    } catch (std::exception const &e) {
        return fmi3Error;
    }

    return fmi3OK;
}

template<class T>
fmi3Status get_v(fmi3Component c, const fmi3ValueReference vr[], size_t nvr, T value[], size_t nValues, std::vector<T> (Model::*getter)(int)) {
    
    try {
        int i = 0;
        
        for (int j = 0; j < nvr; j++) {
            
            auto values = (INSTANCE->*getter)(vr[j]);
            
            for (int k = 0; k < values.size(); k++) {
                
                if (i >= nValues) {
                    // TODO: log error message
                    return fmi3Error;
                }
                
                value[i++] = values[k];
            }
        }
    } catch (std::exception const &e) {
        return fmi3Error;
    }
    
    return fmi3OK;
}

template<class T>
fmi3Status set(fmi3Component c, const fmi3ValueReference vr[], size_t nvr, const T value[], size_t nValues, void (Model::*setter)(int, T)) {
    
    try {
        for (int i = 0; i < nvr; i++) {
            (INSTANCE->*setter)(vr[i], value[i]);
        }
    } catch (std::exception const &e) {
        return fmi3Error;
    }
    
    return fmi3OK;
}

template<class T>
fmi3Status set_v(fmi3Component c, const fmi3ValueReference vr[], size_t nvr, const T value[], size_t nValues, void (Model::*setter)(int, const double*, int*)) {
    
    try {
        int i = 0;

        for (int j = 0; j < nvr; j++) {
            
            (INSTANCE->*setter)(vr[j], value, &i);

            if (i >= nValues) {
                // TODO: log error message
                return fmi3Error;
            }

        }
    } catch (std::exception const &e) {
        return fmi3Error;
    }

    return fmi3OK;
}

extern "C" {
    
    const char* fmi3GetTypesPlatform(void) { return fmi3TypesPlatform; }
    
    const char* fmi3GetVersion(void) { return fmi3Version; }
    
    fmi3Status  fmi3SetDebugLogging(fmi3Component c,
                                    fmi3Boolean loggingOn,
                                    size_t nCategories,
                                    const fmi3String categories[]) {
        return fmi3Error;
    }
    
    /* Creation and destruction of FMU instances and setting debug status */
    fmi3Component fmi3Instantiate(fmi3String  instanceName,
                                  fmi3Type    fmuType,
                                  fmi3String  fmuGUID,
                                  fmi3String  fmuResourceLocation,
                                  const fmi3CallbackFunctions* functions,
                                  fmi3Boolean visible,
                                  fmi3Boolean loggingOn) {
        auto component = new MODEL_NAME();
        
        component->m_logger = reinterpret_cast<void *>(functions->logger);
        
        component->initialize();
        
        return component;
    }
    
    void fmi3FreeInstance(fmi3Component c) {
        delete INSTANCE;
    }
    
    /* Enter and exit initialization mode, terminate and reset */
    fmi3Status fmi3SetupExperiment(fmi3Component c,
                                   fmi3Boolean toleranceDefined,
                                   fmi3Float64 tolerance,
                                   fmi3Float64 startTime,
                                   fmi3Boolean stopTimeDefined,
                                   fmi3Float64 stopTime) {
        return fmi3OK;
    }
    
    fmi3Status fmi3EnterInitializationMode(fmi3Component c) {
        INSTANCE->enterInitializationMode();
        return fmi3OK;
    }
    
    fmi3Status fmi3ExitInitializationMode(fmi3Component c) {
        INSTANCE->exitInitializationMode();
        return fmi3OK;
    }
    
    fmi3Status fmi3Terminate(fmi3Component c) {
        return fmi3OK;
    }
    
    fmi3Status fmi3Reset(fmi3Component c) {
        return fmi3Error;
    }
    
    /* Getting and setting variable values */
    fmi3Status fmi3GetFloat32(fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              fmi3Float32 value[], size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3GetFloat64(fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              fmi3Float64 value[], size_t nValues) {
        
        return get_v(c, vr, nvr, value, nValues, &Model::getDouble);
    }
    
    fmi3Status fmi3GetInt8   (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              fmi3Int8 value[], size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3GetUInt8  (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              fmi3UInt8 value[], size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3GetInt16  (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              fmi3Int16 value[], size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3GetUInt16 (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              fmi3UInt16 value[], size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3GetInt32  (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              fmi3Int32 value[], size_t nValues) {
        return get_v(c, vr, nvr, value, nValues, &Model::getInt32);
    }
    
    fmi3Status fmi3GetUInt32 (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              fmi3UInt32 value[], size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3GetInt64  (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              fmi3Int64 value[], size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3GetUInt64 (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              fmi3UInt64 value[], size_t nValues) {
        return get_v(c, vr, nvr, value, nValues, &Model::getUInt64);
    }
    
    fmi3Status fmi3GetBoolean(fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              fmi3Boolean value[], size_t nValues) {
        try {
            for (int i = 0; i < nvr; i++) {
                value[i] = INSTANCE->getBool(vr[i])[0] ? fmi3True : fmi3False;
            }
        } catch (std::exception const &e) {
            return fmi3Error;
        }
        
        return fmi3OK;
    }
    
    fmi3Status fmi3GetString (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              fmi3String value[], size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3GetBinary (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              size_t size[], fmi3Binary value[], size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3SetFloat32(fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              const fmi3Float32 value[], size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3SetFloat64(fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              const fmi3Float64 value[], size_t nValues) {
        return set_v(c, vr, nvr, value, nValues, &Model::setDouble);
    }
    
    fmi3Status fmi3SetInt8   (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              const fmi3Int8 value[], size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3SetUInt8  (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              const fmi3UInt8 value[], size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3SetInt16  (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              const fmi3Int16 value[], size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3SetUInt16 (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              const fmi3UInt16 value[], size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3SetInt32  (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              const fmi3Int32 value[], size_t nValues) {
        return set(c, vr, nvr, value, nValues, &Model::setInt32);
    }
    
    fmi3Status fmi3SetUInt32 (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              const fmi3UInt32 value[], size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3SetInt64  (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              const fmi3Int64 value[], size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3SetUInt64 (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              const fmi3UInt64 value[], size_t nValues) {
        return set(c, vr, nvr, value, nValues, &Model::setUInt64);
    }
    
    fmi3Status fmi3SetBoolean(fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              const fmi3Boolean value[], size_t nValues) {
        try {
            for (int i = 0; i < nvr; i++) {
                INSTANCE->setBool(vr[i], value[i] != fmi3False);
            }
        } catch (std::exception const &e) {
            return fmi3Error;
        }
        
        return fmi3OK;
    }
    
    fmi3Status fmi3SetString (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              const fmi3String value[], size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3SetBinary (fmi3Component c,
                              const fmi3ValueReference vr[], size_t nvr,
                              const size_t size[], const fmi3Binary value[], size_t nValues) {
        return fmi3Error;
    }
    
    /* Getting Variable Dependency Information */
    
    fmi3Status fmi3GetNumberOfVariableDependencies(fmi3Component c,
                                                   fmi3ValueReference vr,
                                                   size_t* nDependencies) {
        return fmi3Error;
    }
    
    fmi3Status fmi3GetVariableDependencies(fmi3Component c,
                                           fmi3ValueReference vrDependent,
                                           size_t elementIndexDependent[],
                                           fmi3ValueReference vrIndependent[],
                                           size_t elementIndexIndependent[],
                                           fmi3DependencyKind dependencyType[],
                                           size_t nDependencies) {
        return fmi3Error;
    }
    
    /* Getting and setting the internal FMU state */
    
    fmi3Status fmi3GetFMUstate (fmi3Component c, fmi3FMUstate* FMUstate) {
        return fmi3Error;
    }
    fmi3Status fmi3SetFMUstate (fmi3Component c, fmi3FMUstate  FMUstate) {
        return fmi3Error;
    }
    fmi3Status fmi3FreeFMUstate(fmi3Component c, fmi3FMUstate* FMUstate) {
        return fmi3Error;
    }
    
    fmi3Status fmi3SerializedFMUstateSize(fmi3Component c,
                                          fmi3FMUstate  FMUstate,
                                          size_t* size) {
        return fmi3Error;
    }
    
    fmi3Status fmi3SerializeFMUstate     (fmi3Component c,
                                          fmi3FMUstate  FMUstate,
                                          fmi3Byte serializedState[],
                                          size_t size) {
        return fmi3Error;
    }
    
    fmi3Status fmi3DeSerializeFMUstate   (fmi3Component c,
                                          const fmi3Byte serializedState[],
                                          size_t size,
                                          fmi3FMUstate* FMUstate) {
        return fmi3Error;
    }
    
    /* Getting partial derivatives */
    
    fmi3Status fmi3GetDirectionalDerivative(fmi3Component c,
                                            const fmi3ValueReference vrUnknown[],
                                            size_t nUnknown,
                                            const fmi3ValueReference vrKnown[],
                                            size_t nKnown,
                                            const fmi3Float64 dvKnown[],
                                            size_t nDvKnown,
                                            fmi3Float64 dvUnknown[],
                                            size_t nDvUnknown) {
        return fmi3Error;
    }
    
    /***************************************************
     Types for Functions for FMI3 for Model Exchange
     ****************************************************/
    
    /* Enter and exit the different modes */
    
    fmi3Status fmi3EnterEventMode(fmi3Component c) {
        return fmi3OK;
    }
    
    fmi3Status fmi3NewDiscreteStates(fmi3Component c,
                                     fmi3EventInfo* eventInfo) {
        
        bool valuesOfContinuousStatesChanged = false;
        double nextEventTime = DBL_MAX;
        
        INSTANCE->update(&valuesOfContinuousStatesChanged, &nextEventTime);
        
        eventInfo->newDiscreteStatesNeeded = fmi3False;
        eventInfo->valuesOfContinuousStatesChanged = fmi3True;
        eventInfo->nextEventTimeDefined = eventInfo->nextEventTime < DBL_MAX;
        eventInfo->nextEventTime = nextEventTime;
        
        return fmi3OK;
    }
    
    fmi3Status fmi3EnterContinuousTimeMode(fmi3Component c) {
        return fmi3OK;
    }
    
    fmi3Status fmi3CompletedIntegratorStep(fmi3Component c,
                                           fmi3Boolean noSetFMUStatePriorToCurrentPoint,
                                           fmi3Boolean* enterEventMode,
                                           fmi3Boolean* terminateSimulation) {
        return fmi3OK;
    }
    
    /* Providing independent variables and re-initialization of caching */
    
    fmi3Status fmi3SetTime(fmi3Component c, fmi3Float64 time) {
        
        INSTANCE->setTime(time);
        
        return fmi3OK;
    }
    
    fmi3Status fmi3SetContinuousStates(fmi3Component c,
                                       const fmi3Float64 x[],
                                       size_t nx) {
        
        INSTANCE->setContinuousStates(x);
        
        return fmi3OK;
    }
    
    /* Evaluation of the model equations */
    
    fmi3Status fmi3GetDerivatives(fmi3Component c,
                                  fmi3Float64 derivatives[],
                                  size_t nx) {
        
        INSTANCE->getDerivatives(derivatives);
        
        return fmi3OK;
    }
    
    fmi3Status fmi3GetEventIndicators(fmi3Component c,
                                      fmi3Float64 eventIndicators[],
                                      size_t ni) {
        
        INSTANCE->getEventIndicators(eventIndicators);
        
        return fmi3OK;
    }
    
    fmi3Status fmi3GetContinuousStates(fmi3Component c, fmi3Float64 x[], size_t nx) {
        
        INSTANCE->getContinuousStates(x);

        return fmi3OK;
    }
    
    fmi3Status fmi3GetNominalsOfContinuousStates(fmi3Component c,
                                                 fmi3Float64 x_nominal[],
                                                 size_t nx) {
        return fmi3Error;
    }
    
    fmi3Status fmi3GetNumberOfEventIndicators(fmi3Component c, size_t* nz) {
        
        *nz = INSTANCE->getNumberOfEventIndicators();
        
        return fmi3OK;
    }
    
    fmi3Status fmi3GetNumberOfContinuousStates(fmi3Component c, size_t* nx) {
        
        *nx = INSTANCE->getNumberOfContinuousStates();
        
        return fmi3OK;
    }
    
    /***************************************************
     Types for Functions for FMI3 for Co-Simulation
     ****************************************************/
    
    /* Simulating the slave */
    
    fmi3Status fmi3SetInputDerivatives(fmi3Component c,
                                       const fmi3ValueReference vr[],
                                       size_t nvr,
                                       const fmi3Int32 order[],
                                       const fmi3Float64 value[],
                                       size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3GetOutputDerivatives(fmi3Component c,
                                        const fmi3ValueReference vr[],
                                        size_t nvr,
                                        const fmi3Int32 order[],
                                        fmi3Float64 value[],
                                        size_t nValues) {
        return fmi3Error;
    }
    
    fmi3Status fmi3DoStep(fmi3Component c,
                          fmi3Float64 currentCommunicationPoint,
                          fmi3Float64 communicationStepSize,
                          fmi3Boolean noSetFMUStatePriorToCurrentPoint) {
        
        auto *slave = static_cast<Slave*>(c);
        
        slave->doStep(currentCommunicationPoint, communicationStepSize, noSetFMUStatePriorToCurrentPoint);
        
        return fmi3OK;
    }
    
    fmi3Status fmi3CancelStep(fmi3Component c) {
        return fmi3Error;
    }
    
    /* Inquire slave status */
    
    fmi3Status fmi3GetDoStepPendingStatus(fmi3Component c,
                                          fmi3Status* status,
                                          fmi3String* message) {
        return fmi3Error;
    }
    
    fmi3Status fmi3GetDoStepDiscardedStatus(fmi3Component c,
                                            fmi3Boolean* terminate,
                                            fmi3Float64* lastSuccessfulTime) {
        return fmi3Error;
    }
    
} // extern "C"

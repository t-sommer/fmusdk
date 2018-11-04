#include <algorithm>  // for std::max()

#include "CVodeSolver.h"
#include "nvector/nvector_serial.h"

//#include <cvode/cvode.h>               /* prototypes for CVODE fcts., consts.  */
//#include <nvector/nvector_serial.h>    /* access to serial N_Vector            */
#include <sunmatrix/sunmatrix_dense.h> /* access to dense SUNMatrix            */
#include <sunlinsol/sunlinsol_dense.h> /* access to dense SUNLinearSolver      */
#include <cvode/cvode_direct.h>        /* access to CVDls interface            */
//#include <sundials/sundials_types.h>   /* defs. of realtype, sunindextype      */


CVodeSolver::CVodeSolver(Model &fmu) : m_fmu(fmu) {
    
    m_cvode_mem = CVodeCreate(CV_BDF, CV_NEWTON);
    
    auto status = CVodeSetUserData(m_cvode_mem, this);
    
    // add a dummy state if model is discrete
    const auto nx = std::max(1, fmu.getNumberOfContinuousStates());
    const auto nz = fmu.getNumberOfEventIndicators();
    auto time = 0.0;
    auto relativeTolerance = 1e-5;
    
    m_x = N_VNew_Serial(nx);
    
    // initialize
    fmu.getContinuousStates(NV_DATA_S(m_x));
    
    m_abstol = N_VNew_Serial(nx);
    
    auto abstol = NV_DATA_S(m_abstol);
    
    for (int i = 0; i < nx; i++) {
        abstol[i] = relativeTolerance;
    }
    
    status = CVodeInit(m_cvode_mem, f, time, m_x);
    
    status = CVodeRootInit(m_cvode_mem, nz, g);
    
    status = CVodeSVtolerances(m_cvode_mem, relativeTolerance, m_abstol);
    
    // create dense SUNMatrix for use in linear solves
    auto A = SUNDenseMatrix(nx, nx);
    
    // create dense SUNLinearSolver object for use by CVode
    SUNLinearSolver LS = SUNDenseLinearSolver(m_x, A);
    
    // all CVDlsSetLinearSolver to attach the matrix and linear solver to CVode
    status = CVDlsSetLinearSolver(m_cvode_mem, LS, A);
  
    status = CVodeSetMaxStep(m_cvode_mem, 1000);
    
    status = CVodeSetMaxNumSteps(m_cvode_mem, 500);
}

void CVodeSolver::reset(double time) {
    m_fmu.getContinuousStates(NV_DATA_S(m_x));
    CVodeReInit(m_cvode_mem, time, m_x);
}


void CVodeSolver::step(double t, double tNext, double *tRet, bool *stateEvent) {
    
    auto flag = CVode(m_cvode_mem, tNext, m_x, tRet, CV_NORMAL);
    
    *stateEvent = flag > 0;
    
//    if not self.discrete:
//        self.get_x(self.px, self.nx)
//
//        tret = realtype(0.0)
//
//# perform one step
//        flag = CVode(self.cvode_mem, tNext, self.x, byref(tret), CV_NORMAL)
//
//        stateEvent = flag > 0
//
//        return stateEvent, tret.value
}

int CVodeSolver::f(realtype t, N_Vector y, N_Vector ydot, void *user_data) {
    
    auto solver = static_cast<CVodeSolver*>(user_data);
    
    auto fmu = &solver->m_fmu;
    
    fmu->setTime(t);
    
    if (fmu->getNumberOfContinuousStates() > 0) {
        // get the states 
        fmu->setContinuousStates(NV_DATA_S(y));
        fmu->getDerivatives(NV_DATA_S(ydot));
    } else {
        // set dummy state and derivative
        NV_DATA_S(y)[0] = 0;
        NV_DATA_S(ydot)[0] = 0;
    }
    
    return 0;
}

int CVodeSolver::g(realtype t, N_Vector y, realtype *gout, void *user_data) {
    
    auto solver = static_cast<CVodeSolver*>(user_data);
    
    auto fmu = &solver->m_fmu;
    
    fmu->setTime(t);
    
    if (fmu->getNumberOfContinuousStates() > 0) {
        fmu->setContinuousStates(NV_DATA_S(y));
    }
    
    if (fmu->getNumberOfEventIndicators() > 0) {
        fmu->getEventIndicators(gout);
    }
    
    return 0;
}




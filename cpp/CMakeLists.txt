cmake_minimum_required (VERSION 3.2)

FUNCTION(cat IN_FILE OUT_FILE)
  file(READ ${IN_FILE} CONTENTS)
  file(APPEND ${OUT_FILE} "${CONTENTS}")
ENDFUNCTION()

project (FMUSDK)

ADD_SUBDIRECTORY(sundials)

set(FMI_VERSION 2 CACHE STRING "FMI Version")
set_property(CACHE FMI_VERSION PROPERTY STRINGS 2 3)

if (WIN32)
   set(FMI_PLATFORM win)
elseif (APPLE)
   set(FMI_PLATFORM darwin)
else ()
   set(FMI_PLATFORM linux)
endif ()

if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
    set (FMI_PLATFORM ${FMI_PLATFORM}64)
else ()
    set (FMI_PLATFORM ${FMI_PLATFORM}32)
endif ()

set(MODEL_NAMES BouncingBall Stair VariableTypes)

if (${FMI_VERSION} STREQUAL "3")
    list(APPEND MODEL_NAMES Arrays)
endif()

foreach(MODEL_NAME ${MODEL_NAMES})

set(TARGET_NAME ${MODEL_NAME})

set(SOURCES
  src/CVodeSolver.cpp
  src/Slave.cpp
  src/Model.cpp
  src/FMI${FMI_VERSION}.cpp
)

file(GLOB SUNDIALS_SOURCES
  "sundials/src/cvode/*.c"
  "sundials/src/sundials/*.c"
  "sundials/src/nvec_ser/*.c"
  "sundials/src/sunmat_band/*.c"
  "sundials/src/sunmat_dense/*.c"
  "sundials/src/sunlinsol_band/*.c"
  "sundials/src/sunlinsol_dense/*.c"
  "sundials/src/sunnonlinsol/newton/*.c"
)

SET(HEADERS
  ${MODEL_NAME}/fmi${FMI_VERSION}.xml
  include/CVodeSolver.h
  include/fmi${FMI_VERSION}/fmi${FMI_VERSION}Functions.h
  include/fmi${FMI_VERSION}/fmi${FMI_VERSION}FunctionTypes.h
  include/fmi${FMI_VERSION}/fmi${FMI_VERSION}TypesPlatform.h
  include/Model.h
  include/Slave.h
  include/Solver.h
  ${MODEL_NAME}/config.h
  ${MODEL_NAME}/${MODEL_NAME}.h
)

add_library(${TARGET_NAME} SHARED ${HEADERS} ${SOURCES} ${SUNDIALS_SOURCES})

#add_dependencies(${TARGET_NAME} NVECTOR_SERIAL)

set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 11)

source_group("sundials" FILES ${SUNDIALS_SOURCES})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist)

target_compile_definitions(${TARGET_NAME} PRIVATE DISABLE_PREFIX)

message("CMAKE_CURRENT_SOURCE_DIR:" ${CMAKE_CURRENT_SOURCE_DIR})
message("CMAKE_CURRENT_BINARY_DIR:" ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(${TARGET_NAME} PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/fmi${FMI_VERSION}"
  "${CMAKE_CURRENT_SOURCE_DIR}/sundials/include"
  "${CMAKE_CURRENT_BINARY_DIR}/sundials/include"
  ${MODEL_NAME}
)

set(FMU_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/temp/${MODEL_NAME})

set_target_properties(${TARGET_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY         "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
  LIBRARY_OUTPUT_DIRECTORY         "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
  LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
  LIBRARY_OUTPUT_DIRECTORY_RELEASE "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
  ARCHIVE_OUTPUT_DIRECTORY         "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
  ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${FMU_BUILD_DIR}/binaries/${FMI_PLATFORM}"
)

set_target_properties(${TARGET_NAME} PROPERTIES PREFIX "")
set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME ${MODEL_NAME})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/${MODEL_NAME}/fmi${FMI_VERSION}.xml
  "${FMU_BUILD_DIR}/modelDescription.xml"
)

#add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
  # "${CMAKE_CURRENT_SOURCE_DIR}/${MODEL_NAME}/${MODEL_NAME}.c"
#   "${FMU_BUILD_DIR}/sources/${MODEL_NAME}.c"
# )

#add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
#  "${CMAKE_CURRENT_SOURCE_DIR}/include/fmuTemplate.c"
#  "${FMU_BUILD_DIR}/sources/fmuTemplate.c"
#)

#add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
#  "${CMAKE_CURRENT_SOURCE_DIR}/include/fmuTemplate.h"
#  "${FMU_BUILD_DIR}/sources/fmuTemplate.h"
#)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_CURRENT_SOURCE_DIR}/dist/${MODEL_NAME}.fmu" --format=zip
  "modelDescription.xml"
  "binaries"
#  "sources"
  WORKING_DIRECTORY ${FMU_BUILD_DIR}
)

endforeach(MODEL_NAME)

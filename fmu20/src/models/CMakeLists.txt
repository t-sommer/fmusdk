cmake_minimum_required (VERSION 3.2)

FUNCTION(cat IN_FILE OUT_FILE)
  file(READ ${IN_FILE} CONTENTS)
  file(APPEND ${OUT_FILE} "${CONTENTS}")
ENDFUNCTION()

project (FMUSDK)

set(FMI_VERSION 2 CACHE STRING "FMI Version")
set_property(CACHE FMI_VERSION PROPERTY STRINGS 1 2 3)

set(FMI_TYPE ME CACHE STRING "FMI Type (FMI 1.0 only)")
set_property(CACHE FMI_TYPE PROPERTY STRINGS ME CS)

if (${FMI_VERSION} GREATER 1)
  set(FMI_TYPE "")
endif ()

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

MESSAGE("FMI_PLATFORM: " ${FMI_PLATFORM})

foreach (MODEL_NAME bouncingBall dq inc values vanDerPol)

set(TARGET_NAME ${MODEL_NAME})

SET(HEADERS
  include/model.h
  include/solver.h
  ${MODEL_NAME}/config.h
)

SET(SOURCES
  src/Model.c
  src/slave.c
  src/FMI${FMI_VERSION}.c
  src/ForwardEuler.c
  ${MODEL_NAME}/${MODEL_NAME}.c
)

add_library(${TARGET_NAME} SHARED
  ${HEADERS}
  ${SOURCES}
  ${MODEL_NAME}/FMI${FMI_VERSION}${FMI_TYPE}.xml
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist)

target_compile_definitions(${TARGET_NAME} PRIVATE DISABLE_PREFIX FMI_VERSION=${FMI_VERSION})

if (${FMI_VERSION} EQUAL 1 AND "${FMI_TYPE}" STREQUAL CS)
  message("CO_SIMULATION")
  target_compile_definitions(${TARGET_NAME} PRIVATE FMI_COSIMULATION)
endif()

target_include_directories(${TARGET_NAME} PRIVATE
  "include"
  "${MODEL_NAME}"
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
  ${CMAKE_CURRENT_SOURCE_DIR}/${MODEL_NAME}/FMI${FMI_VERSION}${FMI_TYPE}.xml
  "${FMU_BUILD_DIR}/modelDescription.xml"
)

# add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
#   "${CMAKE_CURRENT_SOURCE_DIR}/${MODEL_NAME}/${MODEL_NAME}.c"
#   "${FMU_BUILD_DIR}/sources/${MODEL_NAME}.c"
# )
#
# add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
#   "${CMAKE_CURRENT_SOURCE_DIR}/src/model.c"
#   "${FMU_BUILD_DIR}/sources/model.c"
# )
#
# add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
#   "${CMAKE_CURRENT_SOURCE_DIR}/model.h"
#   "${FMU_BUILD_DIR}/sources/model.h"
# )

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_CURRENT_SOURCE_DIR}/dist/${MODEL_NAME}.fmu" --format=zip
  "modelDescription.xml"
  "binaries"
  # "sources"
  WORKING_DIRECTORY ${FMU_BUILD_DIR}
)

endforeach(MODEL_NAME)

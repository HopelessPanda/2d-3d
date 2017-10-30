cmake_minimum_required(VERSION 3.0.0)

set(name disparity_estimator)
GenerateStandardLibrary()
set_property(TARGET ${name} PROPERTY POSITION_INDEPENDENT_CODE ON)

if (BUILD_${name})
  target_link_libraries(${name} VideoImage ME MEstimation_Extended FastWMF)

  if (NOT WIN32)
    target_link_libraries(${name} gomp)
  endif()
endif()

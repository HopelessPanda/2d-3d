cmake_minimum_required(VERSION 3.0.0)

set(name FastWMF)
GenerateStandardLibrary()
set_property(TARGET ${name} PROPERTY POSITION_INDEPENDENT_CODE ON)

if (BUILD_${name})
  find_package(OpenCV REQUIRED)

  target_include_directories(${name} INTERFACE ${OpenCV_INCLUDE_DIRS})
  target_link_libraries(${name} ${OpenCV_LIBS})
endif()

cmake_minimum_required(VERSION 3.0.0)

set(name MEstimation_Extended)
GenerateStandardLibrary()

if (BUILD_${name})
    include_directories("../")
    if(ARCH_x64)
      add_definitions(-DUSE_ASM64)
    endif()

    target_link_libraries(${name} ME VGLibCommon)
endif()

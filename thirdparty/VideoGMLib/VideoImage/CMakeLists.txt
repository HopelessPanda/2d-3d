cmake_minimum_required(VERSION 3.0.0)

set(name VideoImage)
GenerateStandardLibrary()
set_property(TARGET ${name} PROPERTY POSITION_INDEPENDENT_CODE ON)
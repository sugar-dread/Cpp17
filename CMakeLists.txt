cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project name and a few useful settings. Other commands can pick up the results
project(
  Cpp17CompleteGuide
  VERSION 1.0
  DESCRIPTION "C++17 feautres by Josuttis book chapters"
  LANGUAGES CXX)


if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output." ON)
if (FORCE_COLORED_OUTPUT)
    add_compile_options (-fdiagnostics-color=always)
endif ()

# add_subdirectory(1_StructuredBindings)
# add_subdirectory(2_ifAndSwitchInitialization)
# add_subdirectory(3_InlineVariables)
# add_subdirectory(5_MandatoryCopyElision)
# add_subdirectory(6_LambdaExtensions)
# add_subdirectory(7_NewAttributes)
# add_subdirectory(9_CTAD)
add_subdirectory(10_CompileTimeIf)
# add_subdirectory(14_std_optional)
# add_subdirectory(15_std_variant)
# add_subdirectory(15_std_any)
# add_subdirectory(17_std_byte)
# add_subdirectory(18_std_stringviews)
# add_subdirectory(19_fileSystemLib)
# add_subdirectory(22_ParallelSTLAlgorithms)
# add_subdirectory(23_SubstrAndSearch)
# add_subdirectory(24_OtherUtilityFunctionsAlgo)
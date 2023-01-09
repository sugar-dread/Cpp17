cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project name and a few useful settings. Other commands can pick up the results
project(
  C++17Features
  VERSION 1.0
  DESCRIPTION "C++17 standards based on Jason Turner youtube channel"
  LANGUAGES CXX)

set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)

  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output." ON)

if(FORCE_COLORED_OUTPUT)
  add_compile_options(-fdiagnostics-color=always)
endif()

# add_subdirectory(1_std_any)
# add_subdirectory(2_std_invoke)
# add_subdirectory(3_consexpr_if)
add_subdirectory(test)
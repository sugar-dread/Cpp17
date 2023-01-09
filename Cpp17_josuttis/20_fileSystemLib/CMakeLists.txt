
file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

# Tests need to be added as executables first
add_executable(${PROJECT_NAME} ${SOURCES})

# We need this directory, and users of our library will need it too
target_include_directories(${PROJECT_NAME} PRIVATE ${ClassDateProject_SOURCE_DIR}/Inc)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${VERSION_STRING})


add_custom_target("run"
                  COMMENT "Runs the ${PROJECT_NAME} executable"
                  USES_TERMINAL
                  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} DEPENDS ${PROJECT_NAME})

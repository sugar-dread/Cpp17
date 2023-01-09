file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
find_package(CURL REQUIRED)

# Tests need to be added as executables first
add_executable(${PROJECT_NAME} ${SOURCES})

# We need this directory, and users of our library will need it too
target_include_directories(${PROJECT_NAME} PRIVATE ${ClassDateProject_SOURCE_DIR}/Inc)

target_link_libraries(${PROJECT_NAME} PRIVATE curl gumbo)

# target_link_libraries(${PROJECT_NAME} PRIVATE threads)
add_custom_target("run"
    COMMENT "Runs the ${PROJECT_NAME} executable"
    USES_TERMINAL
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} DEPENDS ${PROJECT_NAME})
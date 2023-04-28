add_executable(app main.cpp)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})


find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
target_link_libraries(app PUBLIC app-lib reg-lib PRIVATE Qt6::Core Qt6::Widgets)
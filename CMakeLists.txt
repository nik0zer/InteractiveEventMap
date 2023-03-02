cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
project("interactive_event_map")

file(GLOB SERVER_SRC "server_code/src/*.cpp")
file(GLOB CLIENT_SRC "client_code/src/*.cpp")

add_executable(InteractiveEventMap_client ${CLIENT_SRC})
add_executable(InteractiveEventMap_server ${SERVER_SRC})


find_package(Boost REQUIRED)
target_link_libraries(InteractiveEventMap_client PRIVATE Boost::boost)

target_link_libraries(InteractiveEventMap_server PRIVATE Boost::boost)


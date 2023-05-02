cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
project("interactive_event_map")
SET(MYCOMPILE_FLAGS "-pthread")

SET(CMAKE_C_FLAGS    "${CMAKE_C_FLAGS} ${MYCOMPILE_FLAGS}")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${MYCOMPILE_FLAGS}")

file(GLOB SERVER_SRC "server_code/src/*")
file(GLOB CLIENT_SRC "client_code/src/*")
file(GLOB SQL_SRC    "sql_code/src/*")
file(GLOB GUI_SRC    "GUI/src/*")
file(GLOB HEADERS_TO_MOC GUI/include/ *.h)


find_path(SQLITE_MODERN_CPP_INCLUDE_DIRS "sqlite_modern_cpp.h")

add_executable(InteractiveEventMap_client ${CLIENT_SRC})
add_executable(InteractiveEventMap_server ${SERVER_SRC})
#add_executable(InteractiveEventMap_sql    ${SQL_SRC})
add_executable(InteractiveEventMap_gui    ${GUI_SRC})


target_include_directories(InteractiveEventMap_server PRIVATE "server_code/include")
target_include_directories(InteractiveEventMap_client PRIVATE "client_code/include")
#target_include_directories(InteractiveEventMap_sql PRIVATE    "sql_code/include")
target_include_directories(InteractiveEventMap_gui PRIVATE    "GUI/include")


find_package(Boost REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

target_link_libraries(InteractiveEventMap_gui PRIVATE Qt6::Core Qt6::Widgets)
target_link_libraries(InteractiveEventMap_client PRIVATE Boost::boost Boost::thread) 
target_link_libraries(InteractiveEventMap_server PRIVATE Boost::boost Boost::thread)
#target_link_libraries(InteractiveEventMap_sql PRIVATE Boost::boost Boost::thread unofficial::sqlite3::sqlite3 spdlog::spdlog spdlog::spdlog_header_only)

qt6_wrap_cpp(PROCESSED_MOCS                                                                                                                                                                                                                                                                    
             ${HEADERS_TO_MOC}                                                   
             TARGET InteractiveEventMap_gui
             OPTIONS --no-notes)

target_sources(InteractiveEventMap_gui PRIVATE ${PROCESSED_MOCS})
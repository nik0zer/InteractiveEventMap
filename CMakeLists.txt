cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")
project("interactive_event_map")
SET(MYCOMPILE_FLAGS "-pthread")

SET(CMAKE_C_FLAGS    "${CMAKE_C_FLAGS} ${MYCOMPILE_FLAGS}")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${MYCOMPILE_FLAGS}")

file(GLOB SERVER_SRC "server_code/src/*")
file(GLOB CLIENT_SRC "client_code/src/*")
file(GLOB GUI_SRC    "GUI/src/*" )
file(GLOB HEADERS_TO_MOC GUI/include/ *.h)
file(GLOB SQL_NEW "new_sql/src/*")
file(GLOB CLIENT_SQL "client_sql/src/*")
file(GLOB SERVER_SQL "server_sql/src/*")


add_executable(InteractiveEventMap_server_sql ${SERVER_SRC} ${SERVER_SQL} ${SQL_NEW})
add_executable(InteractiveEventMap_client_sql ${CLIENT_SRC} ${CLIENT_SQL} ${SQL_NEW} ${GUI_SRC})


find_package(Boost REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

target_include_directories(InteractiveEventMap_server_sql PUBLIC "new_sql/include" "server_code/include" "server_sql/include")
target_include_directories(InteractiveEventMap_client_sql PUBLIC "new_sql/include" "client_code/include" "client_sql/include" "GUI/include")

target_link_libraries(InteractiveEventMap_client_sql PUBLIC Boost::boost Boost::thread unofficial::sqlite3::sqlite3 
spdlog::spdlog unofficial::sqlite3::sqlite3 spdlog::spdlog_header_only Qt6::Core Qt6::Widgets)
target_link_libraries(InteractiveEventMap_server_sql PUBLIC Boost::boost Boost::thread unofficial::sqlite3::sqlite3 
spdlog::spdlog spdlog::spdlog_header_only unofficial::sqlite3::sqlite3 Qt6::Core Qt6::Widgets)

find_path(SQLITE_MODERN_CPP_INCLUDE_DIRS "sqlite_modern_cpp.h")
target_include_directories(InteractiveEventMap_client_sql PUBLIC ${SQLITE_MODERN_CPP_INCLUDE_DIRS})
target_include_directories(InteractiveEventMap_server_sql PUBLIC ${SQLITE_MODERN_CPP_INCLUDE_DIRS})
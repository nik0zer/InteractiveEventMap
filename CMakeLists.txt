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
file(GLOB SQL_SRC    "sql_code/src/sql_tools.cpp")
file(GLOB GUI_SRC    "GUI/src/*" )
file(GLOB HEADERS_TO_MOC GUI/include/ *.h)


add_executable(InteractiveEventMap_client "client_main.cpp" ${CLIENT_SRC} ${SQL_SRC} ${GUI_SRC})
add_executable(InteractiveEventMap_server "server_main.cpp" ${CLIENT_SRC} ${SQL_SRC})
add_executable(InteractiveEventMap_sql    "sql_code/src/sql_tools.cpp" "sql_code/src/main.cpp")
# add_executable(InteractiveEventMap_gui    ${GUI_SRC} ${SQL_SRC} )
# add_executable(InteractiveEventMap_gui_sql ${GUI_SRC} "sql_code/src/sql_tools.cpp" )


target_include_directories(InteractiveEventMap_server PUBLIC "server_code/include" "sql_code/include")
target_include_directories(InteractiveEventMap_client PUBLIC "client_code/include" "sql_code/include" "GUI/include")
target_include_directories(InteractiveEventMap_sql PUBLIC    "sql_code/include" "client_code/include")
# target_include_directories(InteractiveEventMap_gui PUBLIC    "GUI/include")
# target_include_directories(InteractiveEventMap_gui_sql PUBLIC    "GUI/include" "sql_code/include")


find_package(Boost REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# target_link_libraries(InteractiveEventMap_gui PUBLIC Qt6::Core Qt6::Widgets)
target_link_libraries(InteractiveEventMap_client PUBLIC Boost::boost Boost::thread
 unofficial::sqlite3::sqlite3 spdlog::spdlog spdlog::spdlog_header_only Qt6::Core Qt6::Widgets) 
target_link_libraries(InteractiveEventMap_server PUBLIC Boost::boost Boost::thread
 unofficial::sqlite3::sqlite3 spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(InteractiveEventMap_sql PUBLIC Boost::boost Boost::thread unofficial::sqlite3::sqlite3 spdlog::spdlog spdlog::spdlog_header_only)
# target_link_libraries(InteractiveEventMap_gui_sql PUBLIC unofficial::sqlite3::sqlite3 spdlog::spdlog spdlog::spdlog_header_only)
# target_link_libraries(InteractiveEventMap_gui_sql PUBLIC Qt6::Core Qt6::Widgets)

# qt6_wrap_cpp(PROCESSED_MOCS                                                                                                                                                                                                                                                                    
#              ${HEADERS_TO_MOC}                                                   
#              TARGET InteractiveEventMap_gui
#              OPTIONS --no-notes)

# target_sources(InteractiveEventMap_gui PUBLIC ${PROCESSED_MOCS})
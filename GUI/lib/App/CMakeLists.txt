add_library(app-lib STATIC
src/listwidget.cpp    src/skeleton.cpp  src/Dialog.cpp )

set(CMAKE_AUTOMOC ON)

target_include_directories(app-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
target_link_libraries(app-lib PRIVATE Qt6::Core Qt6::Widgets)




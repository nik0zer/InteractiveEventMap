add_library(reg-lib STATIC
src/auth_window.cpp   src/Mainwindow.cpp    src/reg_window.cpp ../App/src/listwidget.cpp  ../App/src/skeleton.cpp   )

#target_include_directories(reg-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
target_link_libraries(reg-lib PRIVATE Qt6::Core Qt6::Widgets)
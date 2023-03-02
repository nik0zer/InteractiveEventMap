project("InteractiveEventMap")

file(GLOB SERVER_SRC "server_code/src/*.cpp")
file(GLOB CLIENT_SRC "client_code/src/*.cpp")

add_executable(InteractiveEventMap_client ${CLIENT_SRC})
add_executable(InteractiveEventMap_server ${SERVER_SRC})
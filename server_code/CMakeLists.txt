project("InteractiveEventMap_client")

file(GLOB TARGET_SRC "src/*.cpp")

add_executable(InteractiveEventMap_server ${TARGET_SRC})
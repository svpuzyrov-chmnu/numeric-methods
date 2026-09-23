project("round")

file(GLOB SRC src/*.cpp src/*.cxx)

include_directories(include)

add_executable(${PROJECT_NAME} ${SRC})
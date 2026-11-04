project("mean-square-app")

file(GLOB SRC src/*.cpp src/*.cxx)

include_directories(include)

set(DEPS ${COMMON_DEPS} "plane2d" "mean-square" "plot2d")

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${DEPS})
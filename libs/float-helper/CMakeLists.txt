project("float-helper")

file(GLOB SRC src/*.cpp src/*.cxx)

include_directories(include)

add_library(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${COMMON_DEPS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
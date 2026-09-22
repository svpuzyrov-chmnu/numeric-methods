project("matrix")

file(GLOB SRC src/*.cpp src/*.cxx)

include_directories(include)

add_library(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${COMMON_DEPS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    file(GLOB TEST_SRC tests/*.cpp tests/*.cxx)

    if (TEST_SRC)
        set(TEST_EXE_NAME "${PROJECT_NAME}-tests")

        add_executable(${TEST_EXE_NAME} ${TEST_SRC})

        target_link_libraries(${TEST_EXE_NAME} PRIVATE
                ${PROJECT_NAME}
                Catch2::Catch2WithMain
        )
        list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
        include(Catch)
        catch_discover_tests(${TEST_EXE_NAME})
    endif ()

endif()
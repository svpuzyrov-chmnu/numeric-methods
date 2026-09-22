project("matrix")

file(GLOB src src/*.cpp src/*.cxx)

include_directories(include)

add_library(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${COMMON_DEPS})

if (EXISTS "tests")
    file(GLOB TEST_SRC tests/*.cpp tests/*.cxx)

    if (TEST_SRC)
        set(TEST_EXE_NAME "${PROJECT_NAME}-Test")

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
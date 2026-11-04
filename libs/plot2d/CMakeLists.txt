project("plot2d")

include(FetchContent)

FetchContent_Declare(
        sciplot
        GIT_REPOSITORY https://github.com/sciplot/sciplot.git
        GIT_TAG master
        GIT_SHALLOW TRUE
        SOURCE_SUBDIR __headers_only_no_cmake__
)

FetchContent_MakeAvailable(sciplot)

file(GLOB SRC src/*.cpp src/*.cxx)

include_directories(include)

add_library(${PROJECT_NAME} ${SRC})

add_library(${PROJECT_NAME}::plotter ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} ${COMMON_DEPS} "plane2d")

target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE  ${sciplot_SOURCE_DIR}
)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    file(GLOB TEST_SRC tests/*.cpp tests/*.cxx)

    if (TEST_SRC)
        set(TEST_EXE_NAME "${PROJECT_NAME}-tests")

        add_executable(${TEST_EXE_NAME} ${TEST_SRC})

        target_link_libraries(${TEST_EXE_NAME} PRIVATE
                ${PROJECT_NAME}
                Catch2::Catch2WithMain
        )
        catch_discover_tests(${TEST_EXE_NAME})
    endif ()

endif()
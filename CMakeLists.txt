cmake_minimum_required(VERSION 3.5)

option(SCL_TESTS "Build test executables" OFF)

project(scl)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_STANDARD 17)

add_compile_options(-O2)
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_compile_options(-O1 -g -fsanitize=address -fno-omit-frame-pointer)
endif()

FILE(GLOB scl_sources src/*.c)

add_library(${PROJECT_NAME} STATIC
    ${scl_sources}
)

FILE(GLOB scl_test_sources tests/*.c)
foreach(path ${scl_test_sources})
    string(REPLACE ".c" "" tmp ${path})
    get_filename_component(name ${tmp} NAME)
    add_executable(${name} ${path})
    if(CMAKE_BUILD_TYPE MATCHES "Debug")
        target_link_libraries(${name}
            ${PROJECT_NAME}
            -fsanitize=address
	    m
        )
    else()
        target_link_libraries(${name}
            ${PROJECT_NAME}
	    m
        )
    endif()
endforeach()

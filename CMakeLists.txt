cmake_minimum_required(VERSION 3.10)
project(SuckerTest C)

file(GLOB_RECURSE ALL_C TASK-*/src/*.c)
file(GLOB_RECURSE ALL_H TASK-*/include/*.h)

add_executable(dummy ${ALL_C} ${ALL_H})

foreach(HFILE ${ALL_H})
    get_filename_component(DIR ${HFILE} DIRECTORY)
    include_directories(${DIR})
endforeach()

set_target_properties(dummy PROPERTIES EXCLUDE_FROM_ALL TRUE)

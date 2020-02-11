cmake_minimum_required(VERSION 2.8)

project(MiRoN)

#############################################################

set(MIRON_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(MOOD2BE_PATH "${MIRON_PATH}/MOOD2Be")

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${MIRON_PATH}/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
        RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init failed, please checkout submodules manually")
    endif()
endif()

if(NOT EXISTS "${MOOD2BE_PATH}/Executor")
    message(FATAL_ERROR "The submodules were not downloaded.")
endif()

add_subdirectory(${MOOD2BE_PATH}/Executor)
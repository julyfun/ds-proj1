cmake_minimum_required(VERSION 3.25)
project(project_map)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
include_directories("./include")

add_executable(main
    "src/main.cpp"
    "src/log.cpp"
)

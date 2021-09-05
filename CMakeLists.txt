cmake_minimum_required(VERSION 3.10)

set(projectName "screen-space-reflection")
set(ROOT_DIR ${CMAKE_SOURCE_DIR})

message(${projectName})

include_directories(${ROOT_DIR}/include)
link_directories(${ROOT_DIR}/lib)

add_executable(${projectName} ${ROOT_DIR}/src/main.cpp ${ROOT_DIR}/src/scene.cpp ${ROOT_DIR}/src/glad.c)
target_link_libraries(${projectName} ${ROOT_DIR}/lib/libglfw3.a dl m GL X11 pthread)
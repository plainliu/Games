cmake_minimum_required(VERSION 3.10)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)

add_executable(LearnOpenGL
	"ext/src/glad.c"

	"include/shader.h"
	"src/shader.cpp"
	"src/main.cpp"
)

target_include_directories(LearnOpenGL PUBLIC
	"ext/include"
	"include"
)

target_link_directories(LearnOpenGL PUBLIC "ext/libs")
target_link_libraries(LearnOpenGL PUBLIC
	glfw3.lib
	opengl32.lib
)

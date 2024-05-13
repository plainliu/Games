cmake_minimum_required(VERSION 3.21)

project(VulkanTutorial)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DROOT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

find_package(Vulkan REQUIRED)

add_subdirectory(ThirdParty/glfw)
add_subdirectory(ThirdParty/glm)

# 添加源文件
file(GLOB HEADERS "src/*.h")
file(GLOB SOURCES "src/*.cpp" main.cpp)
add_executable(VulkanTutorial
    ${HEADERS}
    ${SOURCES}
)

target_include_directories(VulkanTutorial
    PUBLIC
        ThirdParty/glfw/include
        ThirdParty/glm
)

# 添加依赖库
target_link_libraries(VulkanTutorial
    Vulkan::Vulkan
    glfw
)

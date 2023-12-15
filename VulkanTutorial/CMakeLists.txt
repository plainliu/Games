cmake_minimum_required(VERSION 3.0)

project(VulkanTutorial)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)

add_subdirectory(ThirdParty/glfw)
add_subdirectory(ThirdParty/glm)

# 添加源文件
add_executable(VulkanTutorial main.cpp)

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

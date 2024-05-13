// #include "src/VulkanEnviroment.h"

#include "src/HelloTriangleApplication.h"

#include <iostream>

int main()
{
    // return VulkanEnviroment();

    HelloTriangleApplication app;

    try
    {
        app.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

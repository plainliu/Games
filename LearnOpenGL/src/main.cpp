#include <glad\glad.h> 
#include <GLFW\glfw3.h>

#include <iostream>

const unsigned int gSreenWidth = 800;
const unsigned int gSreenHeight = 600;

void framebuffer_size_callback( GLFWwindow* window, int width, int height );
void processInput( GLFWwindow* window );

int main( )
{
    // init GLFW window
    glfwInit( );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 ); // opengl 3.3
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // opengl core profile
    //glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE ); // open on MacOs

    GLFWwindow* window = glfwCreateWindow( gSreenWidth, gSreenHeight, "LearnOpenGL", NULL, NULL );
    if ( window == NULL )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        return -1;
    }
    glfwMakeContextCurrent( window );

    if ( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) )
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );

    // Render Loop
    while ( !glfwWindowShouldClose( window ) )
    {
        // input
        processInput( window );

        // render
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );

        // swap & deal events
        glfwSwapBuffers( window );
        glfwPollEvents( );
    }
    glfwTerminate( );

    return 0;
}

void framebuffer_size_callback( GLFWwindow* window, int width, int height )
{
    glViewport( 0, 0, width, height );
}

void processInput( GLFWwindow* window )
{
    if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
        glfwSetWindowShouldClose( window, true );
}

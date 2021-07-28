#include <glad\glad.h> 
#include <GLFW\glfw3.h>

#include <iostream>
#include "shader.h"

const std::string gRootPath = "../../../";

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

    //float vertices[] = {
    //    -0.5f, -0.5f, 0.0f,
    //    0.5f, -0.5f, 0.0f,
    //    0.0f, 0.5f, 0.0f
    //};
    float vertices[] = {
        // position         // color
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // ÓÒÏÂ
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // ×óÏÂ
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // ¶¥²¿
    };

    std::string vspath = gRootPath + "src/shaders/triangleVertex.glsl";
    std::string pspath = gRootPath + "src/shaders/triangleFragment.glsl";
    Shader shader( vspath.c_str( ), pspath.c_str( ) );

    // bind VAO
    unsigned int VAO;
    glGenVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );

    unsigned int VBO;
    glGenBuffers( 1, &VBO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    // position
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), (void*)0 );
    glEnableVertexAttribArray( 0 );
    // color
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), (void*)( 3 * sizeof( float ) ) );
    glEnableVertexAttribArray( 1 );

    // Render Loop
    while ( !glfwWindowShouldClose( window ) )
    {
        // input
        processInput( window );

        // render
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );

        // draw
        shader.use( );

        float timeValue = glfwGetTime( );
        float scaleValue = ( sin( timeValue ) / 2.0f ) + 0.5f;
        shader.setFloat( "scaleValue", scaleValue );

        glBindVertexArray( VAO );
        glDrawArrays( GL_TRIANGLES, 0, 3 );

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

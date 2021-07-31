#include <glad\glad.h> 
#include <GLFW\glfw3.h>

#include <iostream>
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    // trianges
    //float vertices[] = {
    //    // position         // color
    //     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // 右下
    //    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // 左下
    //     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 顶部
    //};
    float vertices[] = {
        // ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // 左下
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // 左上
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    //std::string vspath = gRootPath + "src/shaders/triangleVertex.glsl";
    //std::string pspath = gRootPath + "src/shaders/triangleFragment.glsl";
    std::string vspath = gRootPath + "src/shaders/textureVertex.glsl";
    std::string pspath = gRootPath + "src/shaders/textureFragment.glsl";
    Shader shader( vspath.c_str( ), pspath.c_str( ) );

    // bind VAO
    unsigned int VAO;
    glGenVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );

    unsigned int VBO;
    glGenBuffers( 1, &VBO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    // texture
    unsigned int EBO;
    glGenBuffers( 1, &EBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    //// position
    //glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), (void*)0 );
    //glEnableVertexAttribArray( 0 );
    //// color
    //glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), (void*)( 3 * sizeof( float ) ) );
    //glEnableVertexAttribArray( 1 );

    // position
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), (void*)0 );
    glEnableVertexAttribArray( 0 );
    // color
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), (void*)( 3 * sizeof( float ) ) );
    glEnableVertexAttribArray( 1 );
    // tex
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), (void*)( 6 * sizeof( float ) ) );
    glEnableVertexAttribArray( 2 );

    stbi_set_flip_vertically_on_load( true );

    // texture1
    unsigned int texture1;
    glGenTextures( 1, &texture1 );
    glBindTexture( GL_TEXTURE_2D, texture1 );
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // 加载并生成纹理
    int width, height, nrChannels;
    std::string tex = gRootPath + "assets/container.jpg";
    unsigned char* data = stbi_load( tex.c_str( ), &width, &height, &nrChannels, 0 );
    if ( data )
    {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
        glGenerateMipmap( GL_TEXTURE_2D );
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free( data );
    glBindTexture( GL_TEXTURE_2D, NULL );

    // texture2
    unsigned int texture2;
    glGenTextures( 1, &texture2 );
    glBindTexture( GL_TEXTURE_2D, texture2 );
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // 加载并生成纹理
    //int width, height, nrChannels;
    std::string tex2 = gRootPath + "assets/awesomeface.png";
    unsigned char* data2 = stbi_load( tex2.c_str( ), &width, &height, &nrChannels, 0 );
    if ( data2 )
    {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2 );
        glGenerateMipmap( GL_TEXTURE_2D );
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free( data2 );

    shader.use( ); // 不要忘记在设置uniform变量之前激活着色器程序！
    glUniform1i( glGetUniformLocation( shader.ID, "texture1" ), 0 ); // 手动设置
    shader.setInt( "texture2", 1 ); // 或者使用着色器类设置

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

        //float timeValue = glfwGetTime( );
        //float scaleValue = ( sin( timeValue ) / 2.0f ) + 0.5f;
        //shader.setFloat( "scaleValue", scaleValue );

        //glBindVertexArray( VAO );
        //glDrawArrays( GL_TRIANGLES, 0, 3 );

        glm::mat4 trans = glm::mat4( 1.0f );
        trans = glm::translate( trans, glm::vec3( 0.5f, -0.5f, 0.0f ) );
        trans = glm::rotate( trans, (float)glfwGetTime( ), glm::vec3( 0.0, 0.0, 1.0 ) );
        //trans = glm::scale( trans, glm::vec3( 0.5, 0.5, 0.5 ) );

        unsigned int translocation = glGetUniformLocation( shader.ID, "transform" );
        glUniformMatrix4fv( translocation, 1, GL_FALSE, glm::value_ptr( trans ) );

        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, texture1 );
        glActiveTexture( GL_TEXTURE1 );
        glBindTexture( GL_TEXTURE_2D, texture2 );
        glBindVertexArray( VAO );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

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

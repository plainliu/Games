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

    // vs
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec4 vertexColor;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "   vertexColor = vec4(aColor.xyz, 1.0);\n"
        "}\0";
    unsigned int vertexShader;
    vertexShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
    glCompileShader( vertexShader );

    int  success;
    char infoLog[512];
    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // ps
    const char* fragmentShaderSource = "#version 330 core\n"
        "in vec4 vertexColor;\n"
        "out vec4 FragColor;\n"
        "uniform vec4 ourColor;\n"
        "void main()\n"
        "{\n"
        "   //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "   FragColor = vertexColor * ourColor.y;\n"
        "}\0";
    unsigned int fragmentShader;
    fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
    glCompileShader( fragmentShader );

    // link
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram( );

    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );
    glLinkProgram( shaderProgram );

    glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );
    if ( !success ) {
        glGetProgramInfoLog( shaderProgram, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

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
        glUseProgram( shaderProgram );

        float timeValue = glfwGetTime( );
        float greenValue = ( sin( timeValue ) / 2.0f ) + 0.5f;
        int vertexColorLocation = glGetUniformLocation( shaderProgram, "ourColor" );
        glUniform4f( vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f );

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

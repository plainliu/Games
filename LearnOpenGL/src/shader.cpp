#include "shader.h"

Shader::Shader( const GLchar* vertexPath, const GLchar* fragmentPath )
{
    // 1. 从文件路径中获取顶点/片段着色器
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // 保证ifstream对象可以抛出异常：
    vShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    fShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    try
    {
        // 打开文件
        vShaderFile.open( vertexPath );
        fShaderFile.open( fragmentPath );
        std::stringstream vShaderStream, fShaderStream;
        // 读取文件的缓冲内容到数据流中
        vShaderStream << vShaderFile.rdbuf( );
        fShaderStream << fShaderFile.rdbuf( );
        // 关闭文件处理器
        vShaderFile.close( );
        fShaderFile.close( );
        // 转换数据流到string
        vertexCode = vShaderStream.str( );
        fragmentCode = fShaderStream.str( );
    }
    catch ( std::ifstream::failure e )
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return;
    }
    const char* vShaderCode = vertexCode.c_str( );
    const char* fShaderCode = fragmentCode.c_str( );

    // 2. 编译着色器
    unsigned int vertex, fragment;

    // 顶点着色器
    vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex, 1, &vShaderCode, NULL );
    glCompileShader( vertex );
    checkCompileErrors( vertex, "VERTEX" );

    // 片段着色器
    fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment, 1, &fShaderCode, NULL );
    glCompileShader( fragment );
    checkCompileErrors( fragment, "FRAGMENT" );

    // 着色器程序
    ID = glCreateProgram( );
    glAttachShader( ID, vertex );
    glAttachShader( ID, fragment );
    glLinkProgram( ID );
    checkCompileErrors( ID, "PROGRAM" );

    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    glDeleteShader( vertex );
    glDeleteShader( fragment );
}

void Shader::use( )
{
    glUseProgram( ID );
}

void Shader::setBool( const std::string& name, bool value ) const
{
    glUniform1i( glGetUniformLocation( ID, name.c_str( ) ), (int)value );
}
void Shader::setInt( const std::string& name, int value ) const
{
    glUniform1i( glGetUniformLocation( ID, name.c_str( ) ), value );
}
void Shader::setFloat( const std::string& name, float value ) const
{
    glUniform1f( glGetUniformLocation( ID, name.c_str( ) ), value );
}

void Shader::checkCompileErrors( unsigned int shader, std::string type )
{
    int success;
    char infoLog[1024];

    if ( type != "PROGRAM" )
    {
        glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
            glGetShaderInfoLog( shader, 512, NULL, infoLog );
            std::cout << "ERROR::SHADER::COMPILATION_FAILED of type: " << type << "\n" << infoLog << std::endl;
        };
    }
    else
    {
        glGetProgramiv( shader, GL_LINK_STATUS, &success );
        if ( !success )
        {
            glGetProgramInfoLog( shader, 1024, NULL, infoLog );
            std::cout << "ERROR::SHADER::LINKING_FAILED of type: " << type << "\n" << infoLog << std::endl;
        }
    }
}

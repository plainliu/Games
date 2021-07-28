#include "shader.h"

Shader::Shader( const GLchar* vertexPath, const GLchar* fragmentPath )
{
    // 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ��֤ifstream��������׳��쳣��
    vShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    fShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    try
    {
        // ���ļ�
        vShaderFile.open( vertexPath );
        fShaderFile.open( fragmentPath );
        std::stringstream vShaderStream, fShaderStream;
        // ��ȡ�ļ��Ļ������ݵ���������
        vShaderStream << vShaderFile.rdbuf( );
        fShaderStream << fShaderFile.rdbuf( );
        // �ر��ļ�������
        vShaderFile.close( );
        fShaderFile.close( );
        // ת����������string
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

    // 2. ������ɫ��
    unsigned int vertex, fragment;

    // ������ɫ��
    vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex, 1, &vShaderCode, NULL );
    glCompileShader( vertex );
    checkCompileErrors( vertex, "VERTEX" );

    // Ƭ����ɫ��
    fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment, 1, &fShaderCode, NULL );
    glCompileShader( fragment );
    checkCompileErrors( fragment, "FRAGMENT" );

    // ��ɫ������
    ID = glCreateProgram( );
    glAttachShader( ID, vertex );
    glAttachShader( ID, fragment );
    glLinkProgram( ID );
    checkCompileErrors( ID, "PROGRAM" );

    // ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
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

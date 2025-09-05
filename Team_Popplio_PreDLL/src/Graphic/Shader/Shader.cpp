/******************************************************************************/
/*!
\file   Shader.cpp
\author Team Popplio
\author Hafawati Binte Mohd Ali (100%)
\par    Course : CSD2400
\par    Section : A
\date   2025/02/28 01:50:22 AM (Last Edit)
\brief
        This source file defines the Shader class, which is used for handling
        shader programs in OpenGL. It includes functions for creating, using,
        and setting uniforms for shaders.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#include <pch.h>
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Popplio
{
    /*!*****************************************************************************
    \brief
    Default constructor for the Shader class.

    *******************************************************************************/
    Shader::Shader() : ID(0)
    {
        //default constructor
    }

    /*!*****************************************************************************
    \brief
    Constructor that generates the shader on the fly.

    \param[in] shaderPath
    [std::string] Path to the shader file.

    *******************************************************************************/
    Shader::Shader(const std::string& shaderPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexPath = shaderPath + ".vert";
        std::string fragmentPath = shaderPath + ".frag";

        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();

        }
        catch (std::ifstream::failure& e)
        {
            std::ostringstream error;
            error << "SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
            Logger::Critical(error.str());
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    /*!*****************************************************************************
    \brief
    Activates the shader.

    *******************************************************************************/
    void Shader::Use() const
    {
        glUseProgram(ID);
    }

    /*!*****************************************************************************
    \brief
    Deactivates the shader.

    *******************************************************************************/
    void Shader::UnUse()
    {
        glUseProgram(0);
    }

    /*!*****************************************************************************
    \brief
    Sets a boolean uniform in the shader.

    \param[in] name
    [const std::string&] Name of the uniform.

    \param[in] value
    [bool] Value to set.

    *******************************************************************************/
    void Shader::setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    
    /*!*****************************************************************************
    \brief
    Sets an integer uniform in the shader.

    \param[in] name
    [const std::string&] Name of the uniform.

    \param[in] value
    [int] Value to set.

    *******************************************************************************/
    void Shader::setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    
    /*!*****************************************************************************
    \brief
    Sets a float uniform in the shader.

    \param[in] name
    [const std::string&] Name of the uniform.

    \param[in] value
    [float] Value to set.

    *******************************************************************************/
    void Shader::setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    /*!*****************************************************************************
    \brief
    Sets a mat3 uniform in the shader for model transformation.

    \param[in] name
    [const char*] Name of the uniform.

    \param[in] val
    [glm::mat3 const&] Value to set.

    *******************************************************************************/
    void Shader::SetUniform(const char* name, glm::mat3 const& val) const
    {
        GLint loc = glGetUniformLocation(ID, name);
        if (loc >= 0) {
            glUniformMatrix3fv(loc, 1, GL_FALSE, &val[0][0]);
        }
        else {
            std::ostringstream error;
            error << "Uniform variable " << name << " doesn't exist" << std::endl;
            Logger::Error(error.str());
        }
    }

    /*!*****************************************************************************
    \brief
    Sets an integer uniform in the shader for texture.

    \param[in] name
    [GLchar const*] Name of the uniform.

    \param[in] val
    [GLint] Value to set.

    *******************************************************************************/
    void Shader::SetUniform(GLchar const* name, GLint val) const
    {
        GLint loc = glGetUniformLocation(ID, name);
        if (loc >= 0) {
            glUniform1i(loc, val);
        }
        else {
            std::ostringstream error;
            error << "Uniform variable " << name << " doesn't exist" << std::endl;
            Logger::Error(error.str());
        }
    }

    /*!*****************************************************************************
    \brief
    Sets a mat4 uniform in the shader for TRS.

    \param[in] name
    [const char*] Name of the uniform.

    \param[in] val
    [glm::mat4 const&] Value to set.

    *******************************************************************************/
    //void Shader::SetUniform(GLchar const* name, glm::mat4 const& val)
    //{
    //    GLint loc = glGetUniformLocation(ID, name);
    //    if (loc >= 0) {
    //        glUniformMatrix4fv(loc, 1, GL_FALSE, &val[0][0]);
    //    }
    //    else 
    //    {
    //        std::ostringstream error;
    //        error << "Uniform variable " << name << " doesn't exist" << std::endl;
    //        Logger::Error(error.str());
    //    }
    //}

    /*!*****************************************************************************
    \brief
    Sets a boolean uniform in the shader.

    \param[in] name
    [const std::string&] Name of the uniform.

    \param[in] value
    [bool] Value to set.

    *******************************************************************************/
    void Shader::SetUniform(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    /*!*****************************************************************************
    \brief
    Sets a float uniform in the shader.

    \param[in] name
    [const std::string&] Name of the uniform.

    \param[in] value
    [float] Value to set.

    *******************************************************************************/
    void Shader::SetUniform(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    /*!*****************************************************************************
    \brief
    Sets a vec2 uniform in the shader.

    \param[in] name
    [const std::string&] Name of the uniform.

    \param[in] value
    [const glm::vec2&] Value to set.

    *******************************************************************************/
    void Shader::SetUniform(GLchar const* name, const glm::vec2& value) const
    {
        // Get the uniform location
        GLint loc = glGetUniformLocation(ID, name);
        if (loc >= 0)
        {
            glUniform2f(loc, value.x, value.y);
        }
        else
        {
            std::ostringstream error;
            error << "Uniform variable " << name << " doesn't exist" << std::endl;
            Logger::Error(error.str());
        }
    }

    /*!*****************************************************************************
    \brief
    Sets a vec3 uniform in the shader.

    \param[in] name
    [const std::string&] Name of the uniform.

    \param[in] value
    [const glm::vec3&] Value to set.

    *******************************************************************************/
    void Shader::SetUniform(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    /*!*****************************************************************************
   \brief
   Sets a vec4 uniform in the shader.

   \param[in] name
   [const std::string&] Name of the uniform.

   \param[in] value
   [const glm::vec3&] Value to set.

   *******************************************************************************/
    void Shader::SetUniform(GLchar const* name, const glm::vec4& value) const
    {
        // Get the uniform location
        GLint loc = glGetUniformLocation(ID, name);
        if (loc >= 0)
        {
            glUniform4f(loc, value.x, value.g, value.b, value.a);
        }
        else
        {
            std::ostringstream error;
            error << "Uniform variable " << name << " doesn't exist" << std::endl;
            Logger::Error(error.str());
        }

    }

    /*!*****************************************************************************
    \brief
    Sets an integer uniform in the shader.

    \param[in] name
    [GLchar const*] Name of the uniform.

    \param[in] value
    [GLint] Value to set.

    *******************************************************************************/
    void Shader::SetInteger(GLchar const* name, GLint val) const
    {
        GLint loc = glGetUniformLocation(ID, name);
        if (loc >= 0)
        {
            glUniform1i(loc, val);
        }
        else
        {
            std::ostringstream error;
            error << "Uniform variable " << name << " doesn't exist" << std::endl;
            Logger::Error(error.str());
        }

    }

    /*!*****************************************************************************
    \brief
    Sets a vec3 uniform in the shader.

    \param[in] name
    [GLchar const*] Name of the uniform.

    \param[in] x
    [GLfloat] X value to set.

    \param[in] y
    [GLfloat] Y value to set.

    \param[in] z
    [GLfloat] Z value to set.

    *******************************************************************************/
    void Shader::SetVector3f(GLchar const* name, GLfloat x, GLfloat y, GLfloat z)  const
    {
        GLint loc = glGetUniformLocation(ID, name);

        if (loc >= 0)
        {
            glUniform3f(loc, x, y, z);
        }
        else
        {
            std::ostringstream error;
            error << "Uniform variable " << name << " doesn't exist" << std::endl;
            Logger::Error(error.str());
        }

    }

    /*!*****************************************************************************
    \brief
    Sets a vec3 uniform in the shader.

    \param[in] name
    [GLchar const*] Name of the uniform.

    \param[in] val
    [glm::vec3 const&] Value to set.

    *******************************************************************************/
    void Shader::SetVector3f(GLchar const* name, glm::vec3 const& val) const
    {
        GLint loc = glGetUniformLocation(ID, name);
        if (loc >= 0)
        {
            glUniform3f(loc, val.x, val.y, val.z);
        }
        else
        {
            std::ostringstream error;
            error << "Uniform variable " << name << " doesn't exist" << std::endl;
            Logger::Error(error.str());
        }
    }

    /*!*****************************************************************************
    \brief
    Sets a mat3 uniform in the shader.

    \param[in] name
    [GLchar const*] Name of the uniform.

    \param[in] val
    [glm::mat3 const&] Value to set.

    *******************************************************************************/
    void Shader::SetMatrix3(GLchar const* name, glm::mat3 const& val) const
    {
        GLint loc = glGetUniformLocation(ID, name);
        if (loc >= 0)
        {
            glUniformMatrix3fv(loc, 1, false, glm::value_ptr(val));

        }
        else
        {
            std::ostringstream error;
            error << "Uniform variable " << name << " doesn't exist" << std::endl;
            Logger::Error(error.str());
        }
    }

    /*!*****************************************************************************
    \brief
    Sets a mat4 uniform in the shader.

    \param[in] name
    [GLchar const*] Name of the uniform.

    \param[in] val
    [glm::mat4 const&] Value to set.

    *******************************************************************************/
    void Shader::SetMatrix4(GLchar const* name, glm::mat4 const& val) const
    {
        GLint loc = glGetUniformLocation(ID, name);
        if (loc >= 0)
        {
            glUniformMatrix4fv(loc, 1, false, glm::value_ptr(val));

        }
        else
        {
            std::ostringstream error;
            error << "Uniform variable " << name << " doesn't exist" << std::endl;
            Logger::Error(error.str());
        }
    }
}
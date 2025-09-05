/******************************************************************************/
/*!
\file   Shader.h
\author Team Popplio
\author Hafawati Binte Mohd Ali (100%)
\par    Course : CSD2400
\par    Section : A
\date   2025/02/28 01:50:22 AM (Last Edit)
\brief
        This header file declares the Shader class, which is used for handling
        shader programs in OpenGL. It includes functions for creating, using,
        and setting uniforms for shaders.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

namespace Popplio
{
    /*!*****************************************************************************
    \brief
    Class representing a shader program in OpenGL. It includes functions for
    creating, using, and setting uniforms for shaders.

    *******************************************************************************/
    class Shader
    {
    public:
        /*!*****************************************************************************
        \brief
        Default constructor for the Shader class.

        *******************************************************************************/
        Shader();

        /*!*****************************************************************************
        \brief
        Constructor that generates the shader on the fly.

        \param[in] shaderPath
        [std::string] Path to the shader file.

        *******************************************************************************/
        Shader(const std::string& shaderPath);

        /*!*****************************************************************************
        \brief
        Activates the shader.

        *******************************************************************************/
        void Use() const;

        /*!*****************************************************************************
        \brief
        Deactivates the shader.

        *******************************************************************************/
        void UnUse();

        /*!*****************************************************************************
        \brief
        Sets a boolean uniform in the shader.

        \param[in] name
        [const std::string&] Name of the uniform.

        \param[in] value
        [bool] Value to set.

        *******************************************************************************/
        void setBool(const std::string& name, bool value) const;

        /*!*****************************************************************************
        \brief
        Sets an integer uniform in the shader.

        \param[in] name
        [const std::string&] Name of the uniform.

        \param[in] value
        [int] Value to set.

        *******************************************************************************/
        void setInt(const std::string& name, int value) const;

        /*!*****************************************************************************
        \brief
        Sets a float uniform in the shader.

        \param[in] name
        [const std::string&] Name of the uniform.

        \param[in] value
        [float] Value to set.

        *******************************************************************************/
        void setFloat(const std::string& name, float value) const;

        /*!*****************************************************************************
        \brief
        Sets a mat3 uniform in the shader for model transformation.

        \param[in] name
        [const char*] Name of the uniform.

        \param[in] val
        [glm::mat3 const&] Value to set.

        *******************************************************************************/
        void SetUniform(const char* name, glm::mat3 const& val) const;

        /*!*****************************************************************************
        \brief
        Sets an integer uniform in the shader for texture.

        \param[in] name
        [GLchar const*] Name of the uniform.

        \param[in] val
        [GLint] Value to set.

        *******************************************************************************/
        void SetUniform(GLchar const* name, GLint val) const;

        /*!*****************************************************************************
        \brief
        Sets a mat4 uniform in the shader for TRS.

        \param[in] name
        [const char*] Name of the uniform.

        \param[in] val
        [glm::mat4 const&] Value to set.

        *******************************************************************************/
        //void SetUniform(const char* name, glm::mat4 const& val);

        /*!*****************************************************************************
        \brief
        Sets a boolean uniform in the shader.

        \param[in] name
        [const std::string&] Name of the uniform.

        \param[in] value
        [bool] Value to set.

        *******************************************************************************/
        void SetUniform(const std::string& name, bool value) const;

        /*!*****************************************************************************
        \brief
        Sets a float uniform in the shader.

        \param[in] name
        [const std::string&] Name of the uniform.

        \param[in] value
        [float] Value to set.

        *******************************************************************************/
        void SetUniform(const std::string& name, float value) const;

        /*!*****************************************************************************
        \brief
        Sets a vec2 uniform in the shader.

        \param[in] name
        [const std::string&] Name of the uniform.

        \param[in] value
        [const glm::vec2&] Value to set.

        *******************************************************************************/
        void SetUniform(GLchar const* name, const glm::vec2& value) const;

        /*!*****************************************************************************
        \brief
        Sets a vec3 uniform in the shader.

        \param[in] name
        [const std::string&] Name of the uniform.

        \param[in] value
        [const glm::vec3&] Value to set.

        *******************************************************************************/
        void SetUniform(const std::string& name, const glm::vec3& value) const;

        /*!*****************************************************************************
        \brief
        Sets a vec4 uniform in the shader.

        \param[in] name
        [GLchar const*] Name of the uniform.

        \param[in] value
        [const glm::vec4] Value to set.

        *******************************************************************************/
        void SetUniform(GLchar const* name, const glm::vec4& value) const;

        /*!*****************************************************************************
        \brief
        Sets an integer uniform in the shader.

        \param[in] name
        [GLchar const*] Name of the uniform.

        \param[in] value
        [GLint] Value to set.

        *******************************************************************************/
        void SetInteger(GLchar const* name, GLint value) const;

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
        void SetVector3f(GLchar const* name, GLfloat x, GLfloat y, GLfloat z) const;

        /*!*****************************************************************************
        \brief
        Sets a vec3 uniform in the shader.

        \param[in] name
        [GLchar const*] Name of the uniform.

        \param[in] val
        [glm::vec3 const&] Value to set.

        *******************************************************************************/
        void SetVector3f(GLchar const* name, glm::vec3 const& val) const;

        /*!*****************************************************************************
        \brief
        Sets a mat3 uniform in the shader.

        \param[in] name
        [GLchar const*] Name of the uniform.

        \param[in] val
        [glm::mat3 const&] Value to set.

        *******************************************************************************/
        void SetMatrix3(GLchar const* name, glm::mat3 const& val) const;

        /*!*****************************************************************************
        \brief
        Sets a mat4 uniform in the shader.

        \param[in] name
        [GLchar const*] Name of the uniform.

        \param[in] val
        [glm::mat4 const&] Value to set.

        *******************************************************************************/
        void SetMatrix4(GLchar const* name, glm::mat4 const& val) const;

        unsigned int GetID() const { return ID; };

    private:
        unsigned int ID;

        /*!*****************************************************************************
        \brief
        Utility function for checking shader compilation/linking errors.

        \param[in] shader
        [unsigned int] Shader ID.

        \param[in] type
        [std::string] Type of the shader (e.g., "VERTEX", "FRAGMENT", "PROGRAM").

        *******************************************************************************/
        void checkCompileErrors(unsigned int shader, std::string type)
        {
            int success;
            char infoLog[1024];
            if (type != "PROGRAM")
            {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    Logger::Error("SHADER_COMPILATION_ERROR of type: " + type + "\n" + infoLog + "\n -- --------------------------------------------------- -- ");
                }
            }
            else
            {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    Logger::Error("PROGRAM_LINKING_ERROR of type: " + type + "\n" + infoLog + "\n -- --------------------------------------------------- -- ");
                }
            }
        }
    };
}

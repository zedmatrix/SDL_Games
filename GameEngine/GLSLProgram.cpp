#include <fstream>
#include <cstdio>
#include <vector>
#include <print>
#include <string>
#include "GLSLProgram.hpp"

GLSLProgram::GLSLProgram() : m_numAttributes(0), m_programID(0), m_vertexShaderID(0), m_fragmentShaderID(0) {
    //Empty
}

GLSLProgram::~GLSLProgram() {
    //Empty
}
void GLSLProgram::use() {
    glUseProgram(m_programID);
    for (int i = 0; i < m_numAttributes; i++) {
        glEnableVertexAttribArray(i);
    }
}
void GLSLProgram::unuse() {
    glUseProgram(0);
    for (int i = 0; i < m_numAttributes; i++) {
        glDisableVertexAttribArray(i);
    }
}

void GLSLProgram::compileShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    m_programID = glCreateProgram();
    m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    if (m_vertexShaderID == 0) {
        fatalError("Vertex Shader Failed to be Created!");
    }

    m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    if (m_fragmentShaderID == 0) {
        fatalError("Fragment Shader Failed to be Created!");
    }

    compile(vertexShaderPath, m_vertexShaderID);
    compile(fragmentShaderPath, m_fragmentShaderID);
}

void GLSLProgram::compile(const std::string& filePath, GLuint id) {
    //Open and Load Files
    std::ifstream file(filePath);
    if (file.fail()) {
        std::perror(filePath.c_str());
        fatalError("Failed to Open: " + filePath);
    }

    std::string fileContents{};
    std::string line{};

    while (std::getline(file, line)) {
        fileContents += line + "\n";
    }
    file.close();

    //compile shader
    const char* contentsPtr = fileContents.c_str();
    glShaderSource(id, 1, &contentsPtr, nullptr);
    glCompileShader(id);
    GLint success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
        glDeleteShader(id);
        std::print("{}\n", &errorLog[0]);
        fatalError("Shader " + filePath + "Failed to Compile");
    }

}
void GLSLProgram::addAttribute(const std::string& attributeName) {
    glBindAttribLocation(m_programID, m_numAttributes, attributeName.c_str());
    m_numAttributes++;
}

GLuint GLSLProgram::getUniformLocation(const std::string& uniformName) {
    GLuint location = glGetUniformLocation(m_programID, uniformName.c_str());
    if (location == GL_INVALID_INDEX) {
        fatalError("Uniform " + uniformName + " not found in shader");
    }
    return location;
}
void GLSLProgram::linkShaders() {
    // Attach our shaders to our program
    glAttachShader(m_programID, m_vertexShaderID);
    glAttachShader(m_programID, m_fragmentShaderID);

    // Link our program
    glLinkProgram(m_programID);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint success = 0;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<char> errorLog(maxLength);
        glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(m_programID);
        glDeleteShader(m_vertexShaderID);
        glDeleteShader(m_fragmentShaderID);
        std::print("{}\n", &errorLog[0]);
        fatalError("Shaders Failed to Link");

    }

    glDetachShader(m_programID, m_vertexShaderID);
    glDetachShader(m_programID, m_fragmentShaderID);
    glDeleteShader(m_vertexShaderID);
    glDeleteShader(m_fragmentShaderID);

}

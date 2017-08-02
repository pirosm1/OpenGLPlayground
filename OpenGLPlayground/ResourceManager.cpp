#include <fstream>
#include <vector>

#include "ResourceManager.h"

mkp::ResourceManager::ResourceManager() {}

GLuint mkp::ResourceManager::createShaderProgram(const std::vector<ShaderInfo> shaders) {
    GLint result = GL_FALSE;
    int infoLogLength;

    if (shaders.empty()) {
        fprintf(stderr, "No shaders supplied to createShaderProgram.\n");
        return 0;
    }

    std::vector<GLuint> shaderIdArr(shaders.size(), 0);
    for (int i = 0, l = shaders.size(); i < l; ++i) {
        // Attempt to create and load vertex shader
        GLuint shaderId =
        loadShader(shaders[i].path, result, infoLogLength, shaders[i].type);

        if (!shaderId) {
            fprintf(stderr, "Failed to create shader program.\n");

            break;
        }

        shaderIdArr[i] = shaderId;
    }

    for (GLuint &s : shaderIdArr) {
        if (!s) {
            for (auto &s2 : shaderIdArr) {
                glDeleteShader(s2);
            }

            return 0;
        }
    }

    // create shader program
    GLuint programID = glCreateProgram();

    // Attempt to attach shaders
    for (auto &s : shaderIdArr) {
        fprintf(stdout, "%d\n", s);
        glAttachShader(programID, s);
    }

    // Link the program to shaders
    glLinkProgram(programID);

    // Check program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0) {
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
        fprintf(stderr, "%s\n", &programErrorMessage[0]);

        for (auto &s : shaderIdArr) {
            glDetachShader(programID, s);
            glDeleteShader(s);
        }

        glDeleteProgram(programID);
        ;

        return 0;
    }

    return programID;
}

GLuint mkp::ResourceManager::loadShader(const char *vertex_file_path,
                                        GLint &result,
                                        int &info_log_length,
                                        const GLenum shader_type) {
    // Create string for vertex shader code
    std::string *shaderCode = new std::string();

    // Read in Vertex shader code, on failure error out
    if (!readShaderFile(vertex_file_path, shaderCode)) {
        fprintf(stderr, "Failed to read %s.\n", vertex_file_path);
        return 0;
    };

    // create the shader
    GLuint shaderID = glCreateShader(shader_type);

    if (!shaderID) {
        fprintf(stderr,
                "Failed to create shader. Invalid shader type provided.\n");

        glDeleteShader(shaderID);
        return 0;
    }

    // Attempt to compile the vertex shader
    if (!compileShader(shaderID, *shaderCode, result, info_log_length)) {
        fprintf(stderr, "Failed to compile %s.\n", vertex_file_path);

        glDeleteShader(shaderID);
        return 0;
    }

    return shaderID;
}

int mkp::ResourceManager::readShaderFile(const char *file_path, std::string *output) {
    std::ifstream shaderStream(file_path, std::ios::in);

    // Error out if stream failed to open
    if (!shaderStream.is_open()) {
        fprintf(stderr, "Unable to open %s.\n", file_path);

        return 0;
    }

    // Read the file and concat into output
    std::string line;
    while (getline(shaderStream, line)) {
        *output += "\n" + line;
    }

    return 1;
}

int mkp::ResourceManager::compileShader(GLuint shader_id,
                                        const std::string shader_code,
                                        GLint &result,
                                        int &info_log_length) {
    const char *shaderSourcePointer = shader_code.c_str();
    glShaderSource(shader_id, 1, &shaderSourcePointer, NULL);
    glCompileShader(shader_id);

    // Check shader
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        std::vector<char> shaderErrorMessage(info_log_length + 1);
        glGetShaderInfoLog(shader_id, info_log_length, NULL, &shaderErrorMessage[0]);
        fprintf(stderr, "%s\n", &shaderErrorMessage[0]);

        return 0;
    }
    return 1;
}

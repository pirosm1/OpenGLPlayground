#include <fstream>
#include <vector>

#include "ResourceManager.h"

mkp::ResourceManager::ResourceManager() {}

GLuint mkp::ResourceManager::createShaderProgram(const char *vertex_file_path,
                                                 const char *fragment_file_path) {
    GLint result = GL_FALSE;
    int infoLogLength;

    // Attempt to create and load vertex shader
    GLuint vertexShaderID = loadVertexShader(vertex_file_path, result, infoLogLength);
    // Attempt to create and load fragment shader
    GLuint fragmentShaderID = loadFragmentShader(fragment_file_path, result, infoLogLength);

    // if loading the vertex or fragment failed, error out
    if (!vertexShaderID || !fragmentShaderID) {
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        return 0;
    }

    // create shader program
    GLuint programID = glCreateProgram();

    // Attempt to attach link program to shaders
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0) {
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
        fprintf(stderr, "%s\n", &programErrorMessage[0]);

        glDetachShader(programID, vertexShaderID);
        glDetachShader(programID, fragmentShaderID);
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        glDeleteProgram(programID);

        return 0;
    }

    return programID;
}

GLuint mkp::ResourceManager::loadVertexShader(const char *vertex_file_path,
                                              GLint &result,
                                              int &infoLogLength) {
    // Create string for vertex shader code
    std::string *vertexShaderCode = new std::string();

    // Read in Vertex shader code, on failure error out
    if (!readShaderFile(vertex_file_path, vertexShaderCode)) {
        fprintf(stderr, "Failed to read %s.\n", vertex_file_path);
        return 0;
    };

    // create the Vertex shader
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

    // Attempt to compile the vertex shader
    if (!compileShader(vertexShaderID, *vertexShaderCode, result, infoLogLength)) {
        fprintf(stderr, "Failed to compile %s.\n", vertex_file_path);

        glDeleteShader(vertexShaderID);
        return 0;
    }

    return vertexShaderID;
}

GLuint mkp::ResourceManager::loadFragmentShader(const char *fragment_file_path,
                                                GLint &result,
                                                int &infoLogLength) {
    // Create string for fragment shader code
    std::string *fragmentShaderCode = new std::string();

    // Read in fragment shader code, on failure error out
    if (!readShaderFile(fragment_file_path, fragmentShaderCode)) {
        fprintf(stderr, "Failed to read %s.\n", fragment_file_path);
        return 0;
    };

    // create the fragment shader
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Attempt to compile the fragment shader
    if (!compileShader(fragmentShaderID, *fragmentShaderCode, result, infoLogLength)) {
        fprintf(stderr, "Failed to compile %s.\n", fragment_file_path);

        glDeleteShader(fragmentShaderID);
        return 0;
    }

    return fragmentShaderID;
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
                                        int &infoLogLength) {
    const char *shaderSourcePointer = shader_code.c_str();
    glShaderSource(shader_id, 1, &shaderSourcePointer, NULL);
    glCompileShader(shader_id);

    // Check shader
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> shaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(shader_id, infoLogLength, NULL, &shaderErrorMessage[0]);
        fprintf(stderr, "%s\n", &shaderErrorMessage[0]);

        return 0;
    }
    return 1;
}

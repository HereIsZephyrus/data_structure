//
//  graphing.cpp
//  data_structure
//
//  Created by ChanningTong on 10/22/24.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "graphing.hpp"
#include "window.hpp"

std::map<std::string,pShader > ShaderBucket;
GLchar* filePath(const char* fileName){
    //checkSourceRelevantPath();
    const char * tcbsearchPath ="../../../../../../../../Program/data_strcture/OpenGL/shaders/";
    GLchar* resource = new char[strlen(tcbsearchPath) + strlen(fileName) + 1];
    strcpy(resource, tcbsearchPath);
    strcat(resource, fileName);
    return resource;
}
std::string Shader::readGLSLfile(const GLchar* filePath){
    std::string fileString;
    std::ifstream fileStream;
    fileStream.exceptions(std::ifstream::badbit);
    try {
        fileStream.open(filePath);
        if (!fileStream.is_open())
            std::cerr << "ERROR::SHADER::Failed_TO_READ_SHADERFILE." << std::endl;
        std::stringstream shaderStream;
        shaderStream << fileStream.rdbuf();
        fileStream.close();
        return shaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cerr<<"ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    return "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ";
}
void Shader::attchShader(std::string shader,GLuint type){
    const GLchar* shaderCode = shader.c_str();
    GLint success;
    GLchar infoLog[512];
    GLuint shaderProgram;
    shaderProgram = glCreateShader(type);
    glShaderSource(shaderProgram, 1, &shaderCode, NULL);
    glCompileShader(shaderProgram);
    // Print compile errors if any
    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glAttachShader(program, shaderProgram);
    glDeleteShader(shaderProgram);
}
void Shader::attchShader(const GLchar* path,GLuint type){
    std::string shader = readGLSLfile(path);
    attchShader(shader,type);
}
void Shader::linkProgram(){
    GLint success;
    GLchar infoLog[512];
    glLinkProgram(program);
    // Print linking errors if any
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

Primitive::Primitive(const std::vector<Vertex>& inputVertex,GLenum shp,Shader* inputshader){
    using structVert = std::vector<Vertex>;
    stride = 6;
    shape = shp;
    vertexNum = inputVertex.size();
    vertices = new GLfloat[vertexNum * 6];
    for (size_t i = 0; i < vertexNum; i++){
        vertices[i * 6] = inputVertex[i].position[0];
        vertices[i * 6 + 1] = inputVertex[i].position[1];
        vertices[i * 6 + 2] = inputVertex[i].position[2];
        vertices[i * 6] = inputVertex[i].color[0];
        vertices[i * 6 + 1] = inputVertex[i].color[1];
        vertices[i * 6 + 2] = inputVertex[i].color[2];
    }
    shader = inputshader;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexNum, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*stride, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*stride, (GLvoid*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void Primitive::draw() const{
    //std::cout<<"Draw is running"<<std::endl;
    if (shader == nullptr){
        std::cerr<<"havn't bind shader"<<std::endl;
        return;
    }
    else
        shader ->use();
    glBindVertexArray(VAO);
    glDrawArrays(shape, 0, static_cast<GLsizei>(vertexNum));
    glBindVertexArray(0);
    return;
}

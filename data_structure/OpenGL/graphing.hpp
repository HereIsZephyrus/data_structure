//
//  graphing.hpp
//  data_structure
//
//  Created by ChanningTong on 10/22/24.
//

#ifndef graphing_hpp
#define graphing_hpp

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstring>
#include <string>
#include <map>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};
GLchar* filePath(const char* fileName);
class Shader{
public:
    Shader(const Shader&) = delete;
    void operator=(const Shader&) = delete;
    Shader(){
        this->program = glCreateProgram();
    }
    void use(){
        glUseProgram(program);
    }
    void attchShader(std::string shader,GLuint type);
    void attchShader(const GLchar* path,GLuint type);
    void linkProgram();
private:
    GLuint program;
    std::string readGLSLfile(const GLchar* filePath);
};
class Primitive{
public:
    Primitive(const std::vector<Vertex>& inputVertex,GLenum shp,Shader* shader);
    Primitive(const Primitive&) = delete;
    void operator=(const Primitive&) = delete;
    ~Primitive(){
        delete [] vertices;
        glDeleteVertexArrays(1,&VAO);
        glDeleteBuffers(1,&VBO);
    }
    virtual void draw();
protected:
    GLuint VAO,VBO;
    Shader* shader;
    GLenum shape;
    GLsizei stride;
    size_t vertexNum;
    GLfloat* vertices;
};
typedef std::unique_ptr<Shader> pShader;
extern std::map<std::string,pShader > ShaderBucket;
#endif /* graphing_hpp */

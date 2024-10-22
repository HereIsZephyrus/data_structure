//
//  component.cpp
//  data_structure
//
//  Created by ChanningTong on 10/21/24.
//

#include "component.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../OpenGL/graphing.hpp"
namespace maze{
void Map::generate(){
    srand((unsigned int)time(0));
    GenerateSize();
    std::cout<<"n = "<<n<<",m = "<<m<<std::endl;
    const GLfloat xstart = -boundaryWidth * m / 2, ystart = boundaryWidth * n / 2;
    {//top
        glm::vec3 p1 = glm::vec3(xstart,ystart,0.0);
        glm::vec3 p2 = glm::vec3(xstart + m * boundaryWidth,ystart,0.0);
        boundaryTable.push_back(Vertex(p1,boundaryColor));
        boundaryTable.push_back(Vertex(p2,boundaryColor));
    }
    {//bottom
        glm::vec3 p1 = glm::vec3(xstart,ystart - n * boundaryWidth,0.0);
        glm::vec3 p2 = glm::vec3(xstart + m * boundaryWidth,ystart - n * boundaryWidth,0.0);
        boundaryTable.push_back(Vertex(p1,boundaryColor));
        boundaryTable.push_back(Vertex(p2,boundaryColor));
    }
    {//left
        glm::vec3 p1 = glm::vec3(xstart,ystart - boundaryWidth,0.0);
        glm::vec3 p2 = glm::vec3(xstart,ystart - n * boundaryWidth,0.0);
        boundaryTable.push_back(Vertex(p1,boundaryColor));
        boundaryTable.push_back(Vertex(p2,boundaryColor));
    }
    {//right
        glm::vec3 p1 = glm::vec3(xstart + m * boundaryWidth,ystart,0.0);
        glm::vec3 p2 = glm::vec3(xstart + m * boundaryWidth,ystart - (n-1) * boundaryWidth,0.0);
        boundaryTable.push_back(Vertex(p1,boundaryColor));
        boundaryTable.push_back(Vertex(p2,boundaryColor));
    }
    {
        vector<bool> horizontal;
        for (int i = 0; i < m; i++)
            horizontal.push_back(true);
        horiBoundary.push_back(horizontal);
    }
    for (int i = 1; i <= n-1; i++){
        vector<bool> horizontal;
        for (int j = 0; j < m; j++){
            bool existwall = rand()%10 >= 6;
            horizontal.push_back(existwall);
            if (existwall){
                glm::vec3 p1 = glm::vec3(xstart + j * boundaryWidth,ystart - i * boundaryWidth,0.0);
                glm::vec3 p2 = glm::vec3(xstart + (j+1) * boundaryWidth,ystart - i * boundaryWidth,0.0);
                boundaryTable.push_back(Vertex(p1,boundaryColor));
                boundaryTable.push_back(Vertex(p2,boundaryColor));
            }
        }
        horiBoundary.push_back(horizontal);
    }
    {
        vector<bool> horizontal;
        for (int i = 0; i < m; i++)
            horizontal.push_back(true);
        horiBoundary.push_back(horizontal);
    }
    
    {
        vector<bool> vertical;
        vertical.push_back(false);
        vertical.push_back(false);
        for (int j = 2; j < m; j++){
            bool existwall = rand()%10 >= 6;
            vertical.push_back(existwall);
            if (existwall){
                glm::vec3 p1 = glm::vec3(xstart + j * boundaryWidth,ystart,0.0);
                glm::vec3 p2 = glm::vec3(xstart + j * boundaryWidth,ystart - 1 * boundaryWidth,0.0);
                boundaryTable.push_back(Vertex(p1,boundaryColor));
                boundaryTable.push_back(Vertex(p2,boundaryColor));
            }
        }
        vertical.push_back(true);
        vertBoundary.push_back(vertical);
    }
    for (int i = 1; i < n-1; i++){
        vector<bool> vertical;
        vertical.push_back(true);
        for (int j = 1; j <= m-1; j++){
            bool existwall = rand()%10 >= 6;
            vertical.push_back(existwall);
            if (existwall){
                glm::vec3 p1 = glm::vec3(xstart + j * boundaryWidth,ystart - i * boundaryWidth,0.0);
                glm::vec3 p2 = glm::vec3(xstart + j * boundaryWidth,ystart - (i+1) * boundaryWidth,0.0);
                boundaryTable.push_back(Vertex(p1,boundaryColor));
                boundaryTable.push_back(Vertex(p2,boundaryColor));
            }
        }
        vertical.push_back(true);
        vertBoundary.push_back(vertical);
    }
    {
        vector<bool> vertical;
        vertical.push_back(true);
        for (int j = 1; j < m-1; j++){
            bool existwall = rand()%10 >= 6;
            vertical.push_back(existwall);
            if (existwall){
                glm::vec3 p1 = glm::vec3(xstart + j * boundaryWidth,ystart - (n-1) * boundaryWidth,0.0);
                glm::vec3 p2 = glm::vec3(xstart + j * boundaryWidth,ystart - n * boundaryWidth,0.0);
                boundaryTable.push_back(Vertex(p1,boundaryColor));
                boundaryTable.push_back(Vertex(p2,boundaryColor));
            }
        }
        vertical.push_back(false);
        vertical.push_back(false);
        vertBoundary.push_back(vertical);
    }
    
     //check map
    for (int i = 0; i < n; i++){
        std::cout<<' ';
        for (int j = 0; j < m; j++)
            std::cout<<horiBoundary[i][j]<<' ';
        std::cout<<std::endl;
        for (int j = 0; j < m+1; j++)
            std::cout<<vertBoundary[i][j]<<' ';
        std::cout<<std::endl;
    }
    std::cout<<' ';
    for (int j = 0; j < m; j++)
        std::cout<<horiBoundary[n][j]<<' ';
    std::cout<<std::endl;
}
void Map::clear(){
    horiBoundary.clear(); vertBoundary.clear();
    path.clear();
    n = 0; m = 0;
}
void Map::autostep(){
    
}
bool Map::solve() {
    visited = new bool*[n];
    for (int i = 0; i < n; i++){
        visited[i] = new bool[m];
        for (int j = 0; j < m; j++)
            visited[i][j] = false;
    }
    bool canAccess = DFS(0, 0);
    if (canAccess){
        for (vector<Point>::const_reverse_iterator it = path.rbegin(); it != path.rend(); it++)
            std::cout<<it->first<<' '<<it->second<<std::endl;
        writePathVert();
    }
    for (int i = 0; i < n; i++)
        delete [] visited[i];
    delete [] visited;
    visited = nullptr;
    std::cout<<canAccess<<std::endl;
    return canAccess;
}
void Map::GenerateSize(){
    n = rand() % 8 + 3;
    m = rand() % 8 + 3;
}
bool Map::DFS(unsigned int cx,unsigned int cy){
    visited[cy][cx] = true;
    if (cx == m-1 && cy == n-1){
        path.push_back(Point{cx,cy});
        return true;
    }
    for (int dir = 0; dir < 4; dir++){
        const int nx = cx + dx[dir], ny = cy + dy[dir];
        if (nx < 0 || ny < 0 || nx >= m || ny >= n || visited[ny][nx])
            continue;
        bool goRight = ny > cy && !vertBoundary[ny][nx+1];
        bool goLeft = ny < cy && !vertBoundary[ny][cx+1];
        bool goBottom = nx > cx && !horiBoundary[ny+1][nx];
        bool goTop = nx < cx && !horiBoundary[cy+1][nx];
        bool canTransfer = goRight || goLeft || goTop || goBottom;
        if (canTransfer && DFS(ny,nx)){
            path.push_back(Point{cy,cx});
            return true;
        }
    }
    return false;
}
void InitResource(){
    {
        pShader outside (new Shader());
        outside->attchShader(filePath("maze_vertices.vs"),GL_VERTEX_SHADER);
        outside->attchShader(filePath("maze_outsideboundary.gs"),GL_GEOMETRY_SHADER);
        outside->attchShader(filePath("maze_line.frag"),GL_FRAGMENT_SHADER);
        outside->linkProgram();
        ShaderBucket["outside"] = std::move(outside);
    }
    {
        pShader inside (new Shader());
        inside->attchShader(filePath("maze_vertices.vs"),GL_VERTEX_SHADER);
        inside->attchShader(filePath("maze_insideboundary.gs"),GL_GEOMETRY_SHADER);
        inside->attchShader(filePath("maze_line.frag"),GL_FRAGMENT_SHADER);
        inside->linkProgram();
        ShaderBucket["inside"] = std::move(inside);
    }
    {
        pShader path (new Shader());
        path->attchShader(filePath("maze_vertices.vs"),GL_VERTEX_SHADER);
        path->attchShader(filePath("maze_path.gs"),GL_GEOMETRY_SHADER);
        path->attchShader(filePath("maze_line.frag"),GL_FRAGMENT_SHADER);
        path->linkProgram();
        ShaderBucket["path"] = std::move(path);
    }
    {
        pShader test (new Shader());
        test->attchShader(filePath("test_vertices.vs"),GL_VERTEX_SHADER);
        test->attchShader(filePath("test_line.frag"),GL_FRAGMENT_SHADER);
        test->linkProgram();
        ShaderBucket["test"] = std::move(test);
    }
}
void Map::writePathVert(){
    pathTable.clear();
    const GLfloat semiboundaryWidth = boundaryWidth / 2;
    const GLfloat xstart = -semiboundaryWidth * m, ystart = semiboundaryWidth * n;
    
    for (vector<Point>::const_reverse_iterator it = path.rbegin(); it != path.rend(); it++){
        const GLfloat x = semiboundaryWidth * (2 * (it->first) + 1);
        const GLfloat y = semiboundaryWidth * (2 * (it->second) + 1);
        pathTable.push_back(Vertex(glm::vec3(x,y,0),pathColor));
    }
}
void Path::draw(){
    GLuint EBO;
    BindVertex(EBO);
    if (shader == nullptr){
        std::cerr<<"havn't bind shader"<<std::endl;
        return;
    }
    else
        shader ->use();
    glBindVertexArray(VAO);
    //glDrawArrays(shape, 0, static_cast<GLsizei>(vertexNum));
    glDrawElements(shape, static_cast<GLsizei>(vertexNum), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &EBO);
    return;
}
void Path::BindVertex(GLuint& EBO){
    if (indices != nullptr)
        delete [] indices;
    const GLuint len = back - front;
    indices = new GLuint[len * 2];
    indices[0] = front;
    for (GLuint i = front + 1; i < back - 1; i++){
        const int count = (i - front) * 2 - 1;
        indices[count] = i; indices[count+1] = i;
    }
    indices[len * 2 - 1] = back - 1;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}
}

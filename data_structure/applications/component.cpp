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
#include <cmath>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../OpenGL/graphing.hpp"
namespace maze{
void Map::generate(){
    srand((unsigned int)time(0));
    GenerateSize();
    // std::cout<<"n = "<<n<<",m = "<<m<<std::endl;
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
    
    /*
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
     */
}
void Map::clear(){
    horiBoundary.clear(); vertBoundary.clear();
    boundaryTable.clear();
    path.clear();
    n = 0; m = 0;
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
        //for (vector<Point>::const_reverse_iterator it = path.rbegin(); it != path.rend(); it++)
        //    std::cout<<it->first<<' '<<it->second<<std::endl;
        writePathVert();
    }
    for (int i = 0; i < n; i++)
        delete [] visited[i];
    delete [] visited;
    visited = nullptr;
    return canAccess;
}
void Map::GenerateSize(){
    n = rand() % 8 + 3;
    m = rand() % 8 + 3;
}
bool Map::DFS(unsigned int cy,unsigned int cx){
    visited[cy][cx] = true;
    if (cx == m-1 && cy == n-1){
        path.push_back(Point{cx,cy});
        return true;
    }
    for (int dir = 0; dir < 4; dir++){
        const int nx = cx + dx[dir], ny = cy + dy[dir];
        if (nx < 0 || ny < 0 || nx >= m || ny >= n || visited[ny][nx])
            continue;
        bool goBottom = ny > cy && !horiBoundary[cy+1][cx];
        bool goTop = ny < cy && !horiBoundary[cy][cx];
        bool goRight = nx > cx && !vertBoundary[cy][cx+1];
        bool goLeft = nx < cx && !vertBoundary[cy][cx];
        bool canTransfer = goRight || goLeft || goTop || goBottom;
        if (canTransfer && DFS(ny,nx)){
            path.push_back(Point{cx,cy});
            return true;
        }
    }
    return false;
}
void InitResource(){
    {
        pShader inside (new Shader());
        inside->attchShader(filePath("maze_vertices.vs"),GL_VERTEX_SHADER);
        inside->attchShader(filePath("maze_insideboundary.gs"),GL_GEOMETRY_SHADER);
        inside->attchShader(filePath("maze_line.frag"),GL_FRAGMENT_SHADER);
        inside->linkProgram();
        ShaderBucket["line"] = std::move(inside);
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
    pathTable.push_back(Vertex(glm::vec3(xstart,ystart - semiboundaryWidth,0),pathColor));
    for (vector<Point>::const_reverse_iterator it = path.rbegin(); it != path.rend(); it++){
        const GLfloat x = xstart + semiboundaryWidth * (2 * (it->first) + 1);
        const GLfloat y = ystart - semiboundaryWidth * (2 * (it->second) + 1);
        pathTable.push_back(Vertex(glm::vec3(x,y,0),pathColor));
    }
    pathTable.push_back(Vertex(glm::vec3(xstart + semiboundaryWidth * (2 * m),ystart - semiboundaryWidth * (2 * n - 1),0),pathColor));
}
void Path::draw() const{
    if (shader == nullptr){
        std::cerr<<"havn't bind shader"<<std::endl;
        return;
    }
    else
        shader ->use();
    GLuint thicknessLoc = glGetUniformLocation(shader->program, "thickness");
    GLuint transparentLoc = glGetUniformLocation(shader->program, "transparent");
    glUniform1f(thicknessLoc,0.04f);
    glUniform1f(transparentLoc,1.0f);
    glBindVertexArray(VAO);
    glDrawArrays(shape, front, static_cast<GLsizei>(back - front));
    //glDrawElements(shape, static_cast<GLsizei>((back - front - 1) * 2), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    return;
}
void Boundary::draw() const{
    //std::cout<<"Draw is running"<<std::endl;
    if (shader == nullptr){
        std::cerr<<"havn't bind shader"<<std::endl;
        return;
    }
    else
        shader ->use();
    GLuint thicknessLoc = glGetUniformLocation(shader->program, "thickness");
    GLuint transparentLoc = glGetUniformLocation(shader->program, "transparent");
    glUniform1f(thicknessLoc,0.02f);
    const double currentTime = glfwGetTime(),lastTime = Recorder::getRecord().mazeShowUpStartTime;
    glUniform1f(transparentLoc,std::min(static_cast<float>(currentTime - lastTime)/1.0f,1.0f));
    glBindVertexArray(VAO);
    glDrawArrays(shape, 0, static_cast<GLsizei>(vertexNum));
    glBindVertexArray(0);
}
bool Path::autostep(){
    const double currentTime = glfwGetTime(),lastTime = Recorder::getRecord().autoNextTime;
    const float t = 0.2f * std::abs(static_cast<int>(back - 5)) / vertexNum + 0.05;
    if (currentTime - lastTime > t){
        Recorder::getRecord().autoNextTime = currentTime;
        if (!Showed())
            StepIn();
        else if (!Eliminated())
            StepOut();
        else
            return false;
    }else{
        const double ratio = (currentTime - lastTime) / t;
        if (!Showed()){
            const GLfloat dx = vertices[back * 6] - vertices[(back-1) * 6];
            const GLfloat dy = vertices[back * 6 + 1] - vertices[(back-1) * 6 + 1];
            const glm::vec3 color = {vertices[3],vertices[4],vertices[5]};
            const glm::vec3 p1 = {vertices[(back-1) * 6],vertices[(back-1) * 6 + 1],0};
            const glm::vec3 p2 = {
                static_cast<GLfloat>(vertices[(back-1) * 6] + dx * ratio),
                static_cast<GLfloat>(vertices[(back-1) * 6 + 1] + dy * ratio),0,
            };
            std::vector<Vertex> tempLine{Vertex(p1,color),Vertex(p2,color)};
            Path tempPath(tempLine);
            tempPath.StepIn();
            tempPath.draw();
        }else if (!Eliminated() && front){
            const GLfloat dx = vertices[front * 6] - vertices[(front-1) * 6];
            const GLfloat dy = vertices[front * 6 + 1] - vertices[(front-1) * 6 + 1];
            const glm::vec3 color = {vertices[3],vertices[4],vertices[5]};
            const glm::vec3 p1 = {vertices[front * 6],vertices[front * 6 + 1],0};
            const glm::vec3 p2 = {
                static_cast<GLfloat>(vertices[front * 6] - dx * (1-ratio)),
                static_cast<GLfloat>(vertices[front * 6 + 1] - dy * (1-ratio)),0,
            };
            std::vector<Vertex> tempLine{Vertex(p1,color),Vertex(p2,color)};
            Path tempPath(tempLine);
            tempPath.StepIn();
            tempPath.draw();
        }
    }
    return true;
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        Recorder::getRecord().toStepOver = true;
    if (key == GLFW_KEY_A && action == GLFW_PRESS){
        Recorder::getRecord().autoStepping = !Recorder::getRecord().autoStepping;
        Recorder::getRecord().autoNextTime = glfwGetTime();
    }
}
void Clear(Path*& path, Boundary*& boundary,Map& map){
    delete boundary;
    delete path;
    map.clear();
    map.generate();
    //std::cout<<Recorder::getRecord().mazeShowUpStartTime<<' '<<glfwGetTime();
    Recorder::getRecord().mazeShowUpStartTime = glfwGetTime();
    boundary = new Boundary(map.getBoundaryVert());
    if (map.solve())
        path = new Path(map.getPathVert());
    else
        path = nullptr;
}
}

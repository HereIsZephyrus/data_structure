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
#include <memory>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../OpenGL/graphing.hpp"
#include "../OpenGL/window.hpp"
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
void InitResource(GLFWwindow *& window){
    glfwSetKeyCallback(window, keyCallback);
    {
        pShader line (new Shader());
        line->attchShader(filePath("vertices.vs"),GL_VERTEX_SHADER);
        line->attchShader(filePath("maze_line.gs"),GL_GEOMETRY_SHADER);
        line->attchShader(filePath("line.frag"),GL_FRAGMENT_SHADER);
        line->linkProgram();
        ShaderBucket["line"] = std::move(line);
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
    shader ->use();
    GLuint thicknessLoc = glGetUniformLocation(shader->program, "thickness");
    GLuint transparentLoc = glGetUniformLocation(shader->program, "transparent");
    glUniform1f(thicknessLoc,0.04f);
    glUniform1f(transparentLoc,1.0f);
    glBindVertexArray(VAO);
    glDrawArrays(shape, front, static_cast<GLsizei>(back - front));
    glBindVertexArray(0);
}
void Boundary::draw() const{
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
namespace binarytree {
std::vector<std::unique_ptr<Ball>> balls;
std::unique_ptr<Arrow> arrow = nullptr;
void Ball::draw() const{
    shader ->use();
    GLuint radiusLoc = glGetUniformLocation(shader->program, "radius");
    GLuint transparentLoc = glGetUniformLocation(shader->program, "transparent");
    glUniform1f(radiusLoc,radius);
    glUniform1f(transparentLoc,1.0f);
    glBindVertexArray(VAO);
    glDrawArrays(shape, 0, 1);
    glBindVertexArray(0);
}
void Ball::move(){
    constexpr float timeRatio = 0.01;
    constexpr float fuss = 0.99;
    vertices[0] += timeRatio * v.vx;
    vertices[1] += timeRatio * v.vy;
    const GLfloat lowbound = -1 + radius, upBound = 1 - radius;
    if (vertices[0] < lowbound){vertices[0] = lowbound*2 - vertices[0];v.vx = - v.vx;}
    if (vertices[1] < lowbound){vertices[1] = lowbound*2 - vertices[1];v.vy = - v.vy;}
    if (vertices[0] > upBound){vertices[0] = upBound*2 - vertices[0];v.vx = - v.vx;}
    if (vertices[1] > upBound){vertices[1] = upBound*2 - vertices[1];v.vy = - v.vy;}
    //v.vx *= fuss; v.vy *= fuss;
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexNum * 6, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*stride, (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*stride, (GLvoid*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void Ball::collideWith(const Ball* rhs){
    constexpr float coefficientOfRestitution = 1.0f;
    const GLfloat dx = vertices[0] - rhs->getX(),dy = vertices[1] - rhs->getY();
    const GLfloat distance = std::sqrt(dx * dx + dy * dy);
    const GLfloat nx = dx / distance,ny = dy / distance;
    const GLfloat relativeVelocityX = rhs->getV().vx - v.vx;
    const GLfloat relativeVelocityY = rhs->getV().vy - v.vy;
    const GLfloat velocityAlongNormal = relativeVelocityX * nx + relativeVelocityY * ny;
    if (velocityAlongNormal > 0) return;
    const float impulse = -(1 + coefficientOfRestitution) * velocityAlongNormal;
    v.vx += impulse * nx;
    v.vy += impulse * ny;
    float overlap = radius + rhs->getR() - distance;
    vertices[0] -= nx * overlap * 0.5f;
    vertices[1] -= ny * overlap * 0.5f;
}
void Arrow::draw() const{
    shader ->use();
    GLuint transparentLoc = glGetUniformLocation(shader->program, "transparent");
    glUniform1f(transparentLoc,1.0f);
    glBindVertexArray(VAO);
    glDrawArrays(shape, 0, 2);
    glBindVertexArray(0);
}
void InitResource(GLFWwindow *& window){
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetCursorPosCallback(window, cursorCallback);
    {
        pShader ball (new Shader());
        ball->attchShader(filePath("vertices.vs"),GL_VERTEX_SHADER);
        ball->attchShader(filePath("binarytree_ball.gs"),GL_GEOMETRY_SHADER);
        ball->attchShader(filePath("line.frag"),GL_FRAGMENT_SHADER);
        ball->linkProgram();
        ShaderBucket["ball"] = std::move(ball);
    }
    {
        pShader arrow (new Shader());
        arrow->attchShader(filePath("vertices.vs"),GL_VERTEX_SHADER);
        arrow->attchShader(filePath("binarytree_arrow.gs"),GL_GEOMETRY_SHADER);
        arrow->attchShader(filePath("line.frag"),GL_FRAGMENT_SHADER);
        arrow->linkProgram();
        ShaderBucket["arrow"] = std::move(arrow);
    }
    {
        pShader line (new Shader());
        line->attchShader(filePath("vertices.vs"),GL_VERTEX_SHADER);
        line->attchShader(filePath("maze_line.gs"),GL_GEOMETRY_SHADER);
        line->attchShader(filePath("line.frag"),GL_FRAGMENT_SHADER);
        line->linkProgram();
        ShaderBucket["line"] = std::move(line);
    }
    srand((unsigned int)time(0));
}
void Scatter(std::vector<std::unique_ptr<Ball>>& balls,const GLfloat gridsize){
    /*
    WindowParas& windowPara = WindowParas::getInstance();
    const GLfloat xgrid = gridsize / windowPara.WINDOW_WIDTH,ygrid = gridsize / windowPara.WINDOW_HEIGHT;
    for (GLfloat i = -1 + xgrid; i < 1.0f - xgrid; i += xgrid)
        for (GLfloat j = - 1 + ygrid; j < 1.0f - ygrid; j += ygrid){
            const GLfloat randX = i + (rand()%100)/100 * xgrid,randY = j + (rand()%100)/100 * ygrid;
            if (i <= 0 && i + xgrid > 0 && j <=0 && j + ygrid > 0){
                balls.push_back(std::make_unique<Ball>(glm::vec3(randX,randY,0),BallType::power));
                Recorder::getRecord().powerLocation = balls.size()-1;
            }
            else
                balls.push_back(std::make_unique<Ball>(glm::vec3(randX,randY,0),BallType::normal));
        }
*/
    balls.push_back(std::make_unique<Ball>(glm::vec3(0,0,0),BallType::power));
    Recorder::getRecord().powerLocation = 0;
    balls.push_back(std::make_unique<Ball>(glm::vec3(0.4,0.4,0),BallType::normal));
    balls.push_back(std::make_unique<Ball>(glm::vec3(0.1,0.3,0),BallType::normal));
}
bool isColliding(const Ball* a,const Ball* b){
    constexpr GLfloat bias = 0.001;
    const GLfloat dx = a->getX() - b->getX(),dy = a->getY() - b->getY();
    const GLfloat r = a->getR() + b->getR() + bias;
    if (dx * dx + dy * dy <= r)
        return true;
    return false;
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
void mouseCallback(GLFWwindow* window, int button, int action, int mods){
    Recorder& recorder = Recorder::getRecord();
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !recorder.startmoving){
        WindowParas& windowPara = WindowParas::getInstance();
        recorder.stretching = true;
        GLdouble xpos,ypos;
        glfwGetCursorPos(windowPara.window, &xpos, &ypos);
        recorder.strechStartLoc = glm::vec3(windowPara.screen2normalX(xpos),windowPara.screen2normalY(ypos),0.0);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && recorder.stretching){
        recorder.stretching = false;
        recorder.startmoving = true;
        balls[recorder.powerLocation]->setV(arrow->getV());
        arrow = nullptr;
    }
}
void cursorCallback(GLFWwindow* window, double xpos, double ypos){
    Recorder& recorder = Recorder::getRecord();
    if ( recorder.stretching){
        WindowParas& windowPara = WindowParas::getInstance();
        const glm::vec3 strechEndLoc = glm::vec3(windowPara.screen2normalX(xpos),windowPara.screen2normalY(ypos),0.0);
        const glm::vec3 delta = strechEndLoc - recorder.strechStartLoc;
        const int powerInd = recorder.powerLocation;
        const glm::vec3 powerloc = glm::vec3(balls[powerInd]->getX(),balls[powerInd]->getY(),0.0);
        std::cout<<"start:"<<powerloc.x<<' '<<powerloc.y<<std::endl;
        std::cout<<"end:"<<delta.x<<' '<<delta.y<<std::endl;
        const glm::vec3 arrowColor = glm::vec3(1.0,0.0,1.0);
        if (arrow != nullptr)
            arrow = nullptr;
        std::vector<Vertex> arrowVert = {Vertex(powerloc,arrowColor),Vertex(delta,arrowColor)};
        arrow = std::make_unique<Arrow>(arrowVert,Velocity(delta.x,delta.y));
    }
}
}

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
#include <cstring>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../OpenGL/graphing.hpp"
#include "../OpenGL/window.hpp"
#include "../ADT/tree.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "gdal.h"
#include "gdal_priv.h"
#include <gdal_utils.h>
#include "ogrsf_frmts.h"
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
std::vector<std::unique_ptr<BallPara>> balls;
std::unique_ptr<Arrow> arrow = nullptr;
std::unique_ptr<Ball> ballVertices = nullptr;
std::unique_ptr<Ball> powerVertices = nullptr;
std::unique_ptr<BallPara> powerBall = nullptr;
void Ball::draw() const{
    shader ->use();
    GLuint radiusLoc = glGetUniformLocation(shader->program, "radius");
    GLuint transparentLoc = glGetUniformLocation(shader->program, "transparent");
    glUniform1f(radiusLoc,radius);
    glUniform1f(transparentLoc,1.0f);
    glBindVertexArray(VAO);
    glDrawArrays(shape, 0, static_cast<GLsizei>(vertexNum));
    glBindVertexArray(0);
}
void Ball::draw(double timetic){
    Shader *tempShader = ShaderBucket["ball_slide"].get();
    tempShader->use();
    GLuint radiusLoc = glGetUniformLocation(tempShader->program, "radius");
    GLuint transparentLoc = glGetUniformLocation(tempShader->program, "transparent_time");
    glUniform1f(radiusLoc,radius);
    //std::cout<<timetic<<std::endl;
    glUniform1f(transparentLoc,timetic);
    glBindVertexArray(VAO);
    glDrawArrays(shape, 0, static_cast<GLsizei>(vertexNum));
    glBindVertexArray(0);
}
void Ball::update(){
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
bool BallPara::move(){
    const GLfloat rawX = x, rawY = y;
    x += timeRatio * v.vx;
    y += timeRatio * v.vy;
    const GLfloat lowbound = -1 + radius, upBound = 1 - radius;
    if (x < lowbound){x = lowbound*2 - x;v.vx = - v.vx * coefficientOfRestitution;}
    if (y < lowbound){y = lowbound*2 - y;v.vy = - v.vy * coefficientOfRestitution;}
    if (x > upBound){x = upBound*2 - x;v.vx = - v.vx * coefficientOfRestitution;}
    if (y > upBound){y = upBound*2 - y;v.vy = - v.vy * coefficientOfRestitution;}
    v.vx *= fuss; v.vy *= fuss;
    if (indexReference != nullptr){
        const tcb::SpatialRange& range = indexReference->range;
        bool transfer = !(x >= range.minx && x < range.minx + range.width && y >= range.miny && y < range.miny + range.height);
        if (transfer){
            using locs =std::vector<tcb::Point<size_t>>;
            locs& points = indexReference->points;
            locs::const_iterator it = points.begin();
            while (it != points.end()){
                if (it->x == rawX && it->y == rawY)
                    break;
                ++it;
            }
            if (it != points.end())
                points.erase(it);
            return true;
        }
    }
    return false;
}
void BallPara::collideWith(BallPara* rhs){
    const GLfloat dx = (rhs->getX() - x) * 0.75,dy =  rhs->getY() - y;
    const GLfloat distance = std::sqrt(dx * dx + dy * dy);
    const GLfloat nx = dx / distance,ny = dy / distance;
    const GLfloat relativeVelocityX = rhs->getV().vx - v.vx;
    const GLfloat relativeVelocityY = rhs->getV().vy - v.vy;
    const GLfloat velocityAlongNormal = relativeVelocityX * nx + relativeVelocityY * ny;
    if (velocityAlongNormal > 0) return;
    const float impulse = -(1 + coefficientOfRestitution) * velocityAlongNormal;
    float totalMass = mass + rhs->getM();
    float impulsePerMassA = impulse * rhs->getM() / totalMass;
    float impulsePerMassB = impulse * mass / totalMass;
    v.vx -= impulsePerMassA * nx;
    v.vy -= impulsePerMassA * ny;
    (rhs->v).vx += impulsePerMassB * nx;
    (rhs->v).vy += impulsePerMassB * ny;
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
        pShader ball_slide (new Shader());
        ball_slide->attchShader(filePath("ball_slide.vs"),GL_VERTEX_SHADER);
        ball_slide->attchShader(filePath("binarytree_ball_slide.gs"),GL_GEOMETRY_SHADER);
        ball_slide->attchShader(filePath("ball_slide.frag"),GL_FRAGMENT_SHADER);
        ball_slide->linkProgram();
        ShaderBucket["ball_slide"] = std::move(ball_slide);
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
void Scatter(std::vector<std::unique_ptr<BallPara>>& balls,const GLfloat gridsize){
    if (ballVertices!=nullptr)  ballVertices = nullptr;
    if (powerVertices!=nullptr) powerVertices = nullptr;
    if (!balls.empty()) balls.clear();
    if (powerBall != nullptr) powerBall = nullptr;
    WindowParas& windowPara = WindowParas::getInstance();
    const GLfloat xgrid = gridsize / windowPara.WINDOW_WIDTH,ygrid = gridsize / windowPara.WINDOW_HEIGHT;
    std::vector<Vertex> normalVert;
    Vertex powerVert;
    for (GLfloat i = -1.0 + xgrid; i < 1.0f - xgrid; i += xgrid)
        for (GLfloat j = - 1.0 + ygrid; j < 1.0f - ygrid; j += ygrid){
            const GLfloat randX = i + xgrid * (30 + rand()%50)/100,randY = j + ygrid * (30 + rand()%50)/100;
            const glm::vec3 location = glm::vec3(randX,randY,0);
            if (i < 0 && i + xgrid >= 0 && j <0 && j + ygrid >= 0)
                powerVert = Vertex(location,color_setting[BallType::power]);
            else
                normalVert.push_back(Vertex(location,color_setting[BallType::normal]));
        }
    ballVertices = std::make_unique<Ball>(normalVert,radius_setting[BallType::normal],color_setting[BallType::normal]);
    powerVertices = std::make_unique<Ball>(powerVert,radius_setting[BallType::power],color_setting[BallType::power]);
    GLuint counter = 0;
    for (GLfloat i = -1.0 + xgrid; i < 1.0f - xgrid; i += xgrid)
        for (GLfloat j = - 1.0 + ygrid; j < 1.0f - ygrid; j += ygrid){
            if (i < 0 && i + xgrid >= 0 && j <0 && j + ygrid >= 0){
                powerBall = std::make_unique<BallPara>(powerVertices->getX(0),powerVertices->getY(0),BallType::power);
            }
            else{
                balls.push_back(std::make_unique<BallPara>(ballVertices->getX(counter),ballVertices->getY(counter),BallType::normal));
                ++counter;
            }
        }
}
bool isColliding(const BallPara* a,const BallPara* b){
    constexpr GLfloat bias = radius_setting[BallType::normal] / 100;
    const GLfloat dx = (a->getX() - b->getX()) * 0.75,dy = a->getY() - b->getY();
    const GLfloat r = a->getR() + b->getR() + bias;
    if (dx * dx + dy * dy <= r * r)
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
        if (arrow == nullptr){
            recorder.stretching = false;
            return;
        }
        recorder.stretching = false;
        recorder.startmoving = true;
        powerBall->setV(arrow->getV());
        arrow = nullptr;
    }
}
void cursorCallback(GLFWwindow* window, double xpos, double ypos){
    Recorder& recorder = Recorder::getRecord();
    if ( recorder.stretching){
        WindowParas& windowPara = WindowParas::getInstance();
        const glm::vec3 strechEndLoc = glm::vec3(windowPara.screen2normalX(xpos),windowPara.screen2normalY(ypos),0.0);
        const glm::vec3 powerloc = glm::vec3(powerBall->getX(),powerBall->getY(),0.0);
        const glm::vec3 delta = strechEndLoc - recorder.strechStartLoc;
        if (std::abs(delta.x) + std::abs(delta.y) < 0.3) // filter click
            return;
        //std::cout<<"start:"<<powerloc.x<<' '<<powerloc.y<<std::endl;
        //std::cout<<"end:"<<delta.x<<' '<<delta.y<<std::endl;
        const glm::vec3 arrowColor = glm::vec3(1.0,0.0,1.0);
        if (arrow != nullptr)
            arrow = nullptr;
        std::vector<Vertex> arrowVert = {Vertex(powerloc,arrowColor),Vertex(delta,arrowColor)};
        arrow = std::make_unique<Arrow>(arrowVert,Velocity(delta.x,delta.y));
    }
}
void BasicCollideSearch(unsigned long long& counter){
    for (size_t i = 0; i < balls.size(); i++)
        if (isColliding(powerBall.get(),balls[i].get())){
            powerBall->collideWith(balls[i].get());
            ++counter;
        }
    for (size_t i = 0; i < balls.size(); i++)
        for (size_t j = 0; j < balls.size(); j++)
            if (i != j && isColliding(balls[i].get(),balls[j].get())){
                balls[i]->collideWith(balls[j].get());
                ++counter;
                break;
            }
}
void BuildIndexTree(std::shared_ptr<IndexTree> indexTree){
    for (size_t i = 0; i < balls.size(); i++){
        const GLfloat x = balls[i]->getX(), y = balls[i]->getY();
        balls[i]->indexReference = indexTree->insert(x, y, i);
    }
}
void SpatialIndexCollideSeach(std::shared_ptr<IndexTree> indexTree,unsigned long long& counter){
    using tcb::SpatialRange;
    using locs =std::vector<size_t>;
    for (size_t i = 0; i < balls.size(); i++)
        if (isColliding(powerBall.get(),balls[i].get())){
            powerBall->collideWith(balls[i].get());
            ++counter;
        }
    for (size_t i = 0; i < balls.size(); i++){
        const GLfloat x = balls[i]->getX(), y = balls[i]->getY(),r = balls[i]->getR() * 2;
        const GLfloat minx = std::max(-1.0f,x-r), miny = std::max(-1.0f,y-r);
        const GLfloat maxx = std::min(1.0f,x+r), maxy = std::min(1.0f,y+r);
        SpatialRange range(minx, miny,maxx - minx, maxy - miny);
        locs neibors = indexTree->queryRange(range);
        for (locs::const_iterator it = neibors.begin(); it != neibors.end(); it++){
            if ((*it != i) &&isColliding(balls[*it].get(),balls[i].get())){
                balls[i]->collideWith(balls[*it].get());
                ++counter;
            }
        }
    }
}
void DrawGUI(unsigned long long counter){
    using namespace ImGui;
    using std::string;
    Recorder& recorder = Recorder::getRecord();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    NewFrame();
    Begin("panel");
    SetWindowPos("panel", ImVec2(0, 0));
    SetWindowSize("panel", ImVec2(200, 120));
    string countStr = "collide count:"+std::to_string(counter);
    Text("%s",countStr.c_str());
    string sizeStr = "grid size:"+std::to_string(recorder.gridsize);
    Text("%s",sizeStr.c_str());
    SameLine();
    if (Button("+")){
        recorder.gridsize = std::min(recorder.gridsize + 6,(GLuint)54);
        if (!recorder.startmoving){
            recorder.toRestart = true;
            recorder.restartTime = glfwGetTime();
        }
    }
    SameLine();
    if (Button("-")){
        recorder.gridsize = std::max(recorder.gridsize - 6,(GLuint)12);
        if (!recorder.startmoving){
            recorder.toRestart = true;
            recorder.restartTime = glfwGetTime();
        }
    }
    string buttonStr;
    if (recorder.useSpatialIndex)
        buttonStr = "quadtree spatial index";
    else
        buttonStr = "no spatial index";
    if (Button(buttonStr.c_str()))
        recorder.useSpatialIndex = !recorder.useSpatialIndex;
    if (recorder.startmoving && Button("restart")){
        recorder.toRestart = true;
        recorder.restartTime = glfwGetTime();
    }
    End();
}
}

namespace transport{
using std::vector;
using std::string;

void extractVerticesFromGeometry(OGRGeometry* geom, std::vector<glm::vec2>& vertices) {
    std::cout<<geom->getGeometryType()<<std::endl;
    if (geom->getGeometryType() == wkbPoint) {
        // 处理点
        OGRPoint* point = static_cast<OGRPoint*>(geom);
        vertices.push_back({point->getX(), point->getY()});
    } else if (geom->getGeometryType() == wkbLineString) {
        // 处理线段
        OGRLineString* line = static_cast<OGRLineString*>(geom);
        for (int i = 0; i < line->getNumPoints(); ++i) {
            OGRPoint* point = nullptr;
            line->getPoint(i, point);
            vertices.push_back({point->getX(), point->getY()});
        }
    } else if (geom->getGeometryType() == wkbPolygon) {
        // 处理多边形
        OGRPolygon* polygon = static_cast<OGRPolygon*>(geom);
        OGRLinearRing* exteriorRing = polygon->getExteriorRing();
        if (exteriorRing) {
            for (int i = 0; i < exteriorRing->getNumPoints(); ++i) {
                OGRPoint* point = nullptr;
                exteriorRing->getPoint(i, point);
                vertices.push_back({point->getX(), point->getY()});
            }
        }
        // 提取内环
        for (int i = 0; i < polygon->getNumInteriorRings(); ++i) {
            OGRLinearRing* interiorRing = polygon->getInteriorRing(i);
            if (interiorRing) {
                for (int j = 0; j < interiorRing->getNumPoints(); ++j) {
                    OGRPoint* point = nullptr;
                    interiorRing->getPoint(i, point);
                    vertices.push_back({point->getX(), point->getY()});
                }
            }
        }
    } else if (geom->getGeometryType() == wkbMultiPoint) {
        // 处理多点
        OGRMultiPoint* multiPoint = static_cast<OGRMultiPoint*>(geom);
        for (int i = 0; i < multiPoint->getNumGeometries(); ++i) {
            OGRPoint* point = static_cast<OGRPoint*>(multiPoint->getGeometryRef(i));
            vertices.push_back({point->getX(), point->getY()});
        }
    } else if (geom->getGeometryType() == wkbMultiLineString) {
        // 处理多线
        OGRMultiLineString* multiLine = static_cast<OGRMultiLineString*>(geom);
        for (int i = 0; i < multiLine->getNumGeometries(); ++i) {
            OGRLineString* line = static_cast<OGRLineString*>(multiLine->getGeometryRef(i));
            for (int j = 0; j < line->getNumPoints(); ++j) {
                OGRPoint* point = nullptr;
                line->getPoint(i, point);
                vertices.push_back({point->getX(), point->getY()});
            }
        }
    } else if (geom->getGeometryType() == wkbMultiPolygon) {
        // 处理多多边形
        OGRMultiPolygon* multiPolygon = static_cast<OGRMultiPolygon*>(geom);
        for (int i = 0; i < multiPolygon->getNumGeometries(); ++i) {
            OGRPolygon* polygon = static_cast<OGRPolygon*>(multiPolygon->getGeometryRef(i));
            OGRLinearRing* exteriorRing = polygon->getExteriorRing();
            if (exteriorRing) {
                for (int j = 0; j < exteriorRing->getNumPoints(); ++j) {
                    OGRPoint* point = nullptr;
                    exteriorRing->getPoint(i, point);
                    vertices.push_back({point->getX(), point->getY()});
                }
            }
            // 提取内环
            for (int j = 0; j < polygon->getNumInteriorRings(); ++j) {
                OGRLinearRing* interiorRing = polygon->getInteriorRing(j);
                if (interiorRing) {
                    for (int k = 0; k < interiorRing->getNumPoints(); ++k) {
                        OGRPoint* point = nullptr;
                        interiorRing->getPoint(i, point);
                        vertices.push_back({point->getX(), point->getY()});
                    }
                }
            }
        }
    }
}
void loadGeoResource(vector<vector<Vertex>>& pointDataset,string resourcename,const glm::vec3 color){
    GDALDataset* poDataset = (GDALDataset*) GDALOpenEx(resourcename.c_str(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr);

    if (poDataset == nullptr) {
        std::cerr << "Failed to open GeoJSON file: " << resourcename << std::endl;
        return;
    }
    OGRLayer* poLayer = poDataset->GetLayer(0);
    if (poLayer == nullptr) {
        std::cerr << "Failed to get layer from GeoJSON." << std::endl;
        GDALClose(poDataset);
        return;
    }
    
    OGRFeature *poFeature;
    while ((poFeature = poLayer->GetNextFeature()) != nullptr) {
            OGRGeometry* poGeometry = poFeature->GetGeometryRef();
            if (poGeometry) {
                std::vector<glm::vec2> vertices;
                extractVerticesFromGeometry(poGeometry, vertices);

                // 输出顶点数据
                std::cout << "Vertices:" << std::endl;
                for (const auto& vertex : vertices) {
                    std::cout << "(" << vertex.x << ", " << vertex.y << ")" << std::endl;
                }
            }
            OGRFeature::DestroyFeature(poFeature);
        }

    GDALClose(poDataset);
}
}

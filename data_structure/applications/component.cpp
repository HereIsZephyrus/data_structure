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
    const float semiboundaryWidth = 50.0f;
    srand((unsigned int)time(0));
    GenerateSize();
    const float xstart = -semiboundaryWidth * n, ystart = semiboundaryWidth * m;
    {
        vector<bool> horizontal;
        for (int i = 0; i < m; i++)
            horizontal.push_back(true);
        horiBoundary.push_back(horizontal);
    }
    for (int i = 0; i < n-1; i++){
        vector<bool> horizontal;
        for (int j = 0; j < m; j++){
            bool existwall = rand()%10 > 6;
            horizontal.push_back(existwall);
            if (existwall){
                glm::vec3 p1 = glm::vec3(xstart + j * semiboundaryWidth * 2,ystart - (i+1) * semiboundaryWidth * 2,0.0);
                glm::vec3 p2 = glm::vec3(xstart + (j+1) * semiboundaryWidth * 2,ystart - (i+1) * semiboundaryWidth * 2,0.0);
                std::cout<<"horizontal:"<<p1.x<<' '<<p1.y<<' '<<p1.z<<' '<<std::endl;
                std::cout<<"horizontal:"<<p2.x<<' '<<p2.y<<' '<<p2.z<<' '<<std::endl;
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
        for (int i = 0; i < m-1; i++)
            vertical.push_back(true);
        vertBoundary.push_back(vertical);
    }
    for (int i = 0; i < n-2; i++){
        vector<bool> vertical;
        vertical.push_back(true);
        for (int j = 0; j < m-1; j++){
            bool existwall = rand()%10 > 6;
            vertical.push_back(existwall);
            if (existwall){
                glm::vec3 p1 = glm::vec3(xstart + (j+1) * semiboundaryWidth * 2,ystart - i * semiboundaryWidth * 2,0.0);
                glm::vec3 p2 = glm::vec3(xstart + (j+1) * semiboundaryWidth * 2,ystart - (i+1) * semiboundaryWidth * 2,0.0);
                std::cout<<"vertical:"<<p1.x<<' '<<p1.y<<' '<<p1.z<<' '<<std::endl;
                std::cout<<"vertical:"<<p2.x<<' '<<p2.y<<' '<<p2.z<<' '<<std::endl;
                boundaryTable.push_back(Vertex(p1,boundaryColor));
                boundaryTable.push_back(Vertex(p2,boundaryColor));
            }
        }
        vertical.push_back(true);
        vertBoundary.push_back(vertical);
    }
    {
        vector<bool> vertical;
        for (int i = 0; i < m-1; i++)
            vertical.push_back(true);
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
    n = 0; m = 0; current = -1;
}
void Map::autostep(){
    
}
void Map::solve() {
    visited = new bool*[n];
    for (int i = 0; i < n; i++){
        visited[i] = new bool[m];
        for (int j = 0; j < m; j++)
            visited[i][j] = false;
    }
    visited[0][0] = true;
    current = DFS(0, 0)? 0 : -1;
    std::cout<<current<<std::endl;
    if (current >=0 )
        for (vector<Point>::const_reverse_iterator it = path.rbegin(); it != path.rend(); it++)
            std::cout<<it->x<<' '<<it->y<<std::endl;
    for (int i = 0; i < n; i++)
        delete [] visited[i];
    delete [] visited;
    visited = nullptr;
}
void Map::GenerateSize(){
    n = rand() % 8 + 3;
    m = rand() % 8 + 3;
}
bool Map::DFS(unsigned int cx,unsigned int cy){
    if (cx == n-1 && cy == m-1){
        path.push_back(Point(cx,cy));
        return true;
    }
    for (int dir = 0; dir < 4; dir++){
        const int nx = cx + dx[dir], ny = cy + dy[dir];
        if (nx < 0 || ny < 0 || nx >= n || ny >= m || visited[nx][ny])
            continue;
        bool goRight = ny > cy && !vertBoundary[nx][ny];
        bool goLeft = ny < cy && !vertBoundary[cx][ny];
        bool goBottom = nx > cx && !horiBoundary[nx][ny];
        bool goTop = nx < cx && !horiBoundary[nx][cy];
        bool canTransfer = goRight || goLeft || goTop || goBottom;
        if (canTransfer){
            visited[nx][ny] = true;
            if (DFS(nx,ny)){
                path.push_back(Point(cx,cy));
                return true;
            }
            visited[nx][ny] = false;
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
}
}

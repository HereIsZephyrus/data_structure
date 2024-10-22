//
//  component.cpp
//  data_structure
//
//  Created by ChanningTong on 10/21/24.
//

#include "component.hpp"
#include <cstdlib>
#include <ctime>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../OpenGL/graphing.hpp"
namespace maze{
void Map::generate(){
    const float semiboundaryWidth = 5.0f;
    srand((unsigned int)time(0));
    GenerateSize();
    const float xstart = -m * semiboundaryWidth, ystart = n * semiboundaryWidth;
    {
        vector<bool> horizontal;
        for (int i = 0; i < m; i++)
            horizontal.push_back(true);
        horiBoundary.push_back(horizontal);
    }
    for (int i = 0; i < n-1; i++){
        vector<bool> horizontal;
        for (int j = 0; j < m; j++){
            bool existwall = rand() > 0.5;
            horizontal.push_back(existwall);
            if (existwall){
                glm::vec3 p1 = glm::vec3(xstart + j * semiboundaryWidth * 2,ystart - (i+1) * semiboundaryWidth * 2,0.0);
                glm::vec3 p2 = glm::vec3(xstart + (j+1) * semiboundaryWidth * 2,ystart - (i+1) * semiboundaryWidth * 2,0.0);
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
        for (int i = 0; i < m; i++)
            vertical.push_back(true);
        vertBoundary.push_back(vertical);
    }
    for (int i = 0; i < n-1; i++){
        vector<bool> vertical;
        vertical.push_back(true);
        for (int j = 0; j < m-1; j++){
            bool existwall = rand() > 0.5;
            vertical.push_back(existwall);
            if (existwall){
                glm::vec3 p1 = glm::vec3(xstart + (j+1) * semiboundaryWidth * 2,ystart - i * semiboundaryWidth * 2,0.0);
                glm::vec3 p2 = glm::vec3(xstart + (j+1) * semiboundaryWidth * 2,ystart - (i+1) * semiboundaryWidth * 2,0.0);
                boundaryTable.push_back(Vertex(p1,boundaryColor));
                boundaryTable.push_back(Vertex(p2,boundaryColor));
            }
        }
        vertical.push_back(true);
    }
    {
        vector<bool> vertical;
        for (int i = 0; i < m; i++)
            vertical.push_back(true);
        vertical.push_back(false);
        vertBoundary.push_back(vertical);
    }
}
void Map::clear(){
    horiBoundary.clear(); vertBoundary.clear();
    path.clear();
    n = 0; m = 0; current = -1;
}
void Map::autostep(){
    
}
void Map::GenerateSize(){
    n = rand() % 10 + 2;
    m = rand() % 10 + 2;
}
bool Map::DFS(unsigned int cx,unsigned int cy){
    if (cx == m-1 && cy == n-1){
        path.push_back(Point(0,0));
        return true;
    }
    for (int dir = 0; dir < 4; dir++){
        const int nx = cx + dx[dir], ny = cy + dy[dir];
        if (nx < 0 || ny < 0 || nx >= m || ny >= n)
            continue;
        bool goRight = nx > cx && !horiBoundary[nx][ny] && DFS(nx,ny);
        bool goLeft = nx < cx && !horiBoundary[cx][ny] && DFS(nx,ny);
        bool goBottom = ny > cy && !horiBoundary[nx][ny] && DFS(nx,ny);
        bool goTop = ny < cy && !horiBoundary[nx][cy] && DFS(nx,ny);
        if (goRight || goLeft || goTop || goBottom){
            path.push_back(Point(cx,cy));
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
}
}

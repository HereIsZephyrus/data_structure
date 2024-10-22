//
//  component.cpp
//  data_structure
//
//  Created by ChanningTong on 10/21/24.
//

#include "component.hpp"
#include <cstdlib>
#include <ctime>
namespace maze{
void Map::generate(){
    srand((unsigned int)time(0));
    GenerateSize();
    {
        vector<bool> horizontal;
        for (int i = 0; i < m; i++)
            horizontal.push_back(true);
        horiBoundary.push_back(horizontal);
    }
    for (int i = 0; i < n-1; i++){
        vector<bool> horizontal;
        for (int i = 0; i < m; i++)
            horizontal.push_back(rand() > 0.5);
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
        for (int i = 0; i < m-1; i++)
            vertical.push_back(rand() > 0.5);
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
}

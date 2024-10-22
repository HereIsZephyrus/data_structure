//
//  component.hpp
//  data_structure
//
//  Created by ChanningTong on 10/21/24.
//

#ifndef component_hpp
#define component_hpp
#include <vector>
namespace maze{
using std::vector;
class Recorder{
public:
    static Recorder& getRecord(){
        static Recorder instance;
        return instance;
    }
    Recorder(const Recorder&) = delete;
    void operator=(const Recorder&) = delete;
    bool autoStepping;
    bool toStepOver;
private:
    Recorder(){}
};
class Map {
    using matrix = vector<vector<bool>>;
    size_t n,m;
    matrix horiBoundary,vertBoundary;
    struct Point{unsigned int x,y;};
    vector<Point> path;
    int current;
public:
    Map():n(0),m(0){}
    ~Map(){}
    void generate();
    void solve() {current = DFS(0, 0)? 0 : -1;}
    void step(){++current;}
    void clear();
    void autostep();
    bool Finished() const{return current < 0 || current == path.size();}
    const matrix& getHoriBoundary(){return horiBoundary;}
    const matrix& getVertBoundary(){return vertBoundary;}
protected:
    void GenerateSize();
    bool DFS(unsigned int cx,unsigned int cy);
private:
    static constexpr int dx[4] = {-1,0,1,0};
    static constexpr int dy[4] = {0,1,0,-1};
};
}
#endif /* component_hpp */

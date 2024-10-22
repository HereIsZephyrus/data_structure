//
//  component.hpp
//  data_structure
//
//  Created by ChanningTong on 10/21/24.
//

#ifndef component_hpp
#define component_hpp
#include <vector>
#include <map>
#include "../OpenGL/graphing.hpp"
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
    double mazeShowUpStartTime,autoNextTime;
private:
    Recorder(){
        autoStepping = false;
        toStepOver = false;
    }
};

typedef std::pair<int, int> Point;
class Map {
    using matrix = vector<vector<bool>>;
    size_t n,m;
    matrix horiBoundary,vertBoundary;
    vector<Point> path;
    vector<Vertex> boundaryTable,pathTable;
public:
    Map():n(0),m(0){visited = nullptr;}
    ~Map(){}
    void generate();
    bool solve();
    void clear();
    const vector<Vertex>& getBoundaryVert(){return boundaryTable;}
    const vector<Vertex>& getPathVert(){return pathTable;}
    const vector<Point>& getPath(){return path;}
    const size_t getN() const {return n;}
    const size_t getM() const {return m;}
protected:
    void GenerateSize();
    bool DFS(unsigned int cx,unsigned int cy);
private:
    static constexpr int dx[4] = {1,0,-1,0};
    static constexpr int dy[4] = {0,-1,0,1};
    static constexpr glm::vec3 boundaryColor = glm::vec3(1.0f,1.0f,1.0f);
    static constexpr glm::vec3 pathColor = glm::vec3(0,0,1.0f);
    bool** visited;
    void writePathVert();
    static constexpr float boundaryWidth = 0.1f;
};
void InitResource();

class Boundary : public Primitive{
public:
    Boundary(const std::vector<Vertex>& boundVertex):Primitive(boundVertex, GL_LINES, ShaderBucket["line"].get()){}
    void draw() const override;
};
class Path : public Primitive{
public:
    Path(const std::vector<Vertex>& pathVertex):Primitive(pathVertex, GL_LINE_STRIP, ShaderBucket["line"].get()),front(0),back(1){}
    ~Path(){}
    void draw() const override;
    bool Showed() const {return back == vertexNum;}
    bool Eliminated() const {return front == vertexNum-1;}
    void StepIn() {++back;}
    void StepOut() {++front;}
    bool autostep();
protected:
    GLuint front,back;
};
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void Clear(Path*& path, Boundary*& boundary,Map& map);
}
#endif /* component_hpp */

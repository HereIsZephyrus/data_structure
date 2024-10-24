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
#include "../ADT/tree.hpp"
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
void InitResource(GLFWwindow *& window);

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
namespace binarytree{
void InitResource(GLFWwindow *& window);
enum BallType{
    normal = 0,
    power = 1
};
struct Velocity{
    GLfloat vx;
    GLfloat vy;
    Velocity(GLfloat x,GLfloat y){vx = x; vy = y;}
    Velocity(const Velocity& v){vx = v.vx; vy = v.vy;}
};
class Recorder{
public:
    static Recorder& getRecord(){
        static Recorder instance;
        return instance;
    }
    Recorder(const Recorder&) = delete;
    void operator=(const Recorder&) = delete;
    bool stretching,startmoving;
    glm::vec3 strechStartLoc;
private:
    Recorder(){
        stretching = false;
        startmoving = false;
    }
};
class Ball : public Primitive{
public:
    Ball(const std::vector<Vertex>& ballVertices,GLfloat r,glm::vec3 c):
    Primitive(ballVertices,GL_POINTS,ShaderBucket["ball"].get()),radius(r),color(c){}
    Ball(const Vertex ballVertex,GLfloat r,glm::vec3 c):
    Primitive(ballVertex,GL_POINT,ShaderBucket["ball"].get()),radius(r),color(c){}
    void draw() const override;
    void update();
    GLfloat& getX(GLuint num){return vertices[num * 6];}
    GLfloat& getY(GLuint num){return vertices[num * 6 + 1];}
private:
    GLfloat radius;
    glm::vec3 color;
};
class Arrow : public Primitive{
public:
    Arrow(const std::vector<Vertex>& arrowVertex, Velocity v):
    Primitive(arrowVertex,GL_LINES,ShaderBucket["arrow"].get()),arrowV(v){}
    void draw() const override;
    Velocity getV() const {return arrowV;}
private:
    Velocity arrowV;
};
extern std::unique_ptr<Ball> ballVertices,powerVertices;
extern std::unique_ptr<Arrow> arrow;
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow* window, int button, int action, int mods);
void cursorCallback(GLFWwindow* window, double xpos, double ypos);
constexpr glm::vec3 color_setting[2] = {glm::vec3(1.0,1.0,1.0),glm::vec3(1.0,0.0,0.0)};
constexpr GLfloat radius_setting[2] = {0.005,0.01};
constexpr GLfloat m_setting[2] = {1,100};
class BallPara{
public:
    BallPara(GLfloat& xloc, GLfloat& yloc,BallType type):
    radius(radius_setting[type]),mass(m_setting[type]),v(Velocity(0, 0)),x(xloc),y(yloc){}
    GLfloat getX() const {return x;}
    GLfloat getY() const {return y;}
    GLfloat getR() const {return radius;}
    Velocity getV() const {return v;}
    GLfloat getM() const {return mass;}
    void move();
    void setV(Velocity newV) {v = newV;}
    void collideWith(BallPara* rhs);
private:
    GLfloat radius,mass;
    GLfloat &x,&y;
    Velocity v;
    static constexpr float timeRatio = 0.05f;
    static constexpr float fuss = 0.998f;
    static constexpr GLsizei stride = 6;
    static constexpr float coefficientOfRestitution = 1.0f;
};
typedef tcb::QuadTree<size_t>  IndexTree;
extern std::vector<std::unique_ptr<BallPara>> balls;
extern std::unique_ptr<BallPara> powerBall;
void Scatter(std::vector<std::unique_ptr<BallPara>>& balls,const GLfloat gridsize);
bool isColliding(const BallPara* a,const BallPara* b);
void BasicCollideSearch();
void SpatialIndexSeach();
}
#endif /* component_hpp */

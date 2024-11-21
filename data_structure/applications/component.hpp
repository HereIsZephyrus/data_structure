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
#include <gdal.h>
#include <gdal_priv.h>
#include <gdal_utils.h>
#include <ogrsf_frmts.h>
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
    bool stretching,startmoving,useSpatialIndex,toRestart;
    double restartTime;
    GLuint gridsize;
    glm::vec3 strechStartLoc;
private:
    Recorder(){
        stretching = false;
        startmoving = false;
        useSpatialIndex = true;
        toRestart = false;
        gridsize = 18;
    }
};
class Ball : public Primitive{
public:
    Ball(const std::vector<Vertex>& ballVertices,GLfloat r,glm::vec3 c):
    Primitive(ballVertices,GL_POINTS,ShaderBucket["ball"].get()),radius(r),color(c){}
    Ball(const Vertex ballVertex,GLfloat r,glm::vec3 c):
    Primitive(ballVertex,GL_POINT,ShaderBucket["ball"].get()),radius(r),color(c){}
    void draw() const override;
    void draw(double timetic);
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
    using node = tcb::QuadTreeNode<size_t>;
public:
    BallPara(GLfloat& xloc, GLfloat& yloc,BallType type):
    radius(radius_setting[type]),mass(m_setting[type]),v(Velocity(0, 0)),x(xloc),y(yloc){indexReference = nullptr;}
    GLfloat getX() const {return x;}
    GLfloat getY() const {return y;}
    GLfloat getR() const {return radius;}
    Velocity getV() const {return v;}
    GLfloat getM() const {return mass;}
    bool move();
    void setV(Velocity newV) {v = newV;}
    void collideWith(BallPara* rhs);
    node* indexReference;
private:
    GLfloat radius,mass;
    GLfloat &x,&y;
    Velocity v;
    static constexpr float timeRatio = 0.05f;
    static constexpr float fuss = 1.0f;
    static constexpr GLsizei stride = 6;
    static constexpr float coefficientOfRestitution = 1.0f;
};
typedef tcb::QuadTree<size_t>  IndexTree;
extern std::vector<std::unique_ptr<BallPara>> balls;
extern std::unique_ptr<BallPara> powerBall;
void Scatter(std::vector<std::unique_ptr<BallPara>>& balls,const GLfloat gridsize);
bool isColliding(const BallPara* a,const BallPara* b);
void BasicCollideSearch(unsigned long long& counter);
void SpatialIndexCollideSeach(std::shared_ptr<IndexTree> indexTree,unsigned long long& counter);
void BuildIndexTree(std::shared_ptr<IndexTree> indexTree);
void DrawGUI(unsigned long long counter);
}

namespace transport {
using std::vector;
using std::string;
void mouseCallback(GLFWwindow* window, int button, int action, int mods);
class Route : public Primitive{
    void draw() const override;
    int step;
public:
    Route(const vector<Vertex>& vertices):Primitive(vertices, GL_LINES, ShaderBucket["line"].get()){}
    void draw(int steptic){step = steptic; draw();}
};
class Recorder{
public:
    static Recorder& getRecord(){
        static Recorder instance;
        return instance;
    }
    Recorder(const Recorder&) = delete;
    void operator=(const Recorder&) = delete;
    std::map<int,int> mapTopo2Flat,mapFlat2Topo;
    int mapping2Flat(int tomap) const {return mapTopo2Flat.at(tomap);}
    int mapping2Topo(int topoint) const {return mapFlat2Topo.at(topoint);}
    bool toGenerateRoute,toCheckSelect;
    glm::vec2 clickLoc;
    int tickStep;
    int startID,endID;
    double startRoutingTIme;
    std::unique_ptr<Route> basicPath,primPath,kruskalPath;
    //vector<std::pair<int,int>> basicPath,primPath,kruskalPath;
    static constexpr glm::vec3 defaultTrunkColor = glm::vec3(1.0,1.0,1.0);
    static constexpr glm::vec3 basicTrunkColor = glm::vec3(1.0,0.0,0.0);//red
    static constexpr glm::vec3 primTrunkColor = glm::vec3(0.0,1.0,0.0);//green
    static constexpr glm::vec3 kruskalTrunkColor = glm::vec3(1.0,0.0,1.0);//purple
    static constexpr glm::vec3 defaultCityColor  = glm::vec3(0.0,0.0,1.0);
    static constexpr glm::vec3 selectedCityColor = glm::vec3(1.0,0.0,1.0);
    static constexpr double maxCoodx = 135.0;
    static constexpr double minCoodx = 75.0;
    static constexpr double maxCoody = 54.0;
    static constexpr double minCoody = 18.0;
private:
    Recorder(){
        toGenerateRoute = false;
        toCheckSelect = false;
        mapTopo2Flat.clear();
    }
};
void InitResource(GLFWwindow *& window);
class Trunk : public Primitive{
public:
    Trunk(const std::vector<Vertex>& arrowVertex):
    Primitive(arrowVertex,GL_LINES,ShaderBucket["line"].get()){}
    void draw() const override;
};
class Citys : public Primitive{
    static constexpr float radius = 0.015f;
    int startCityID;
public:
    Citys(const std::vector<Vertex>& ballVertices):
    Primitive(ballVertices,GL_POINTS,ShaderBucket["city"].get()),startCityID(-1){}
    void draw() const override;
    bool checkSelect(const glm::vec2& clickloc);
    float calcLength(int start,int end) const{
        const float sx = vertices[start * 6], tx = vertices[end * 6], sy = vertices[start * 6 + 1], ty = vertices[end * 6 + 1];
        return std::sqrt((sx - tx) * (sx - tx) + (sy - ty) * (sy - ty));
    }
    GLfloat getX(int index) const {return vertices[index * 6];}
    GLfloat getY(int index) const {return vertices[index * 6 + 1];}
};
class Station{
    int id;
    GLfloat x,y;
public:
    vector<int> adj;
    vector<float> length;
    Station():id(-1){}
    Station(int id,vector<int> adj) : id(id),adj(adj){}
    Station(int id,std::string neighborStr);
    void setX(GLfloat xx) {x = xx;}
    void setY(GLfloat yy) {y = yy;}
    GLfloat getX() const{return x;}
    GLfloat getY() const{return y;}
    int getID() const {return id;}
};
void loadLineGeoJsonResource(vector<vector<Vertex>>& pointDataset,string resourcename,const glm::vec3 color);
void loadPointGeoJsonResource(vector<Vertex>& pointDataset,vector<Station>& stations,string resourcename,const glm::vec3 color);
void calcDirectLength(vector<Station>& stations,const Citys& citygroup);
bool checkWholeTic();
void solveBasicPath(const int startID,const int endID,const vector<Station>& stations);
void solvePrimPath(const int startID,const int endID,const vector<Station>& stations);
void solveKruskalPath(const int startID,const int endID,const vector<Station>& stations);
void solveThisCityPair(int startID,int endID,const vector<Station>& stations);
}
#endif /* component_hpp */

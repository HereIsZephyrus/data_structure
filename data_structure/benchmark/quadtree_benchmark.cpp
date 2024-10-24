//
//  quadtree_benchmark.cpp
//  data_structure
//
//  Created by ChanningTong on 10/24/24.
//

#include "quadtree_benchmark.hpp"
#include "../applications/component.hpp"
#include "../OpenGL/window.hpp"
#include "../OpenGL/graphing.hpp"
#include "../ADT/tree.hpp"
using namespace binarytree;
using tcb::SpatialRange;

void updateSpatialByQuadTreeIndex(GLfloat gridsize){
    std::vector<std::unique_ptr<BallPara>> balls_bench;
    std::unique_ptr<BallPara> powerBall_bench;
    std::unique_ptr<Ball> ballVertices_bench,powerVertices_bench;
    {
        WindowParas& windowPara = WindowParas::getInstance();
        const GLfloat xgrid = gridsize / windowPara.WINDOW_WIDTH,ygrid = gridsize / windowPara.WINDOW_HEIGHT;
        std::vector<Vertex> normalVert;
        Vertex powerVert;
        for (GLfloat i = -1.0 + xgrid; i < 1.0f - xgrid; i += xgrid)
            for (GLfloat j = - 1.0 + ygrid; j < 1.0f - ygrid; j += ygrid){
                const GLfloat randX = i + (30 + rand()%50)/100 * xgrid,randY = j + (30 + rand()%50)/100 * ygrid;
                const glm::vec3 location = glm::vec3(randX,randY,0);
                if (i < 0 && i + xgrid >= 0 && j <0 && j + ygrid >= 0)
                    powerVert = Vertex(location,color_setting[BallType::power]);
                else
                    normalVert.push_back(Vertex(location,color_setting[BallType::normal]));
            }
        ballVertices_bench = std::make_unique<Ball>(normalVert,radius_setting[BallType::normal],color_setting[BallType::normal]);
        powerVertices_bench = std::make_unique<Ball>(powerVert,radius_setting[BallType::power],color_setting[BallType::power]);
        GLuint counter = 0;
        for (GLfloat i = -1.0 + xgrid; i < 1.0f - xgrid; i += xgrid)
            for (GLfloat j = - 1.0 + ygrid; j < 1.0f - ygrid; j += ygrid){
                if (i < 0 && i + xgrid >= 0 && j <0 && j + ygrid >= 0){
                    powerBall_bench = std::make_unique<BallPara>(powerVertices_bench->getX(0),powerVertices_bench->getY(0),BallType::power);
                    powerBall_bench->setV(Velocity(0.8f, 0.6f));
                }
                else{
                    balls_bench.push_back(std::make_unique<BallPara>(ballVertices_bench->getX(counter),ballVertices_bench->getY(counter),BallType::normal));
                    ++counter;
                }
            }
        
    }
    // build tree
    std::shared_ptr<IndexTree> indexTree = std::make_unique<IndexTree>(SpatialRange(-1.0f,-1.0f,2.0f,2.0f),10);
    for (size_t i = 0; i < balls_bench.size(); i++){
        const GLfloat x = balls_bench[i]->getX(), y = balls_bench[i]->getY();
        balls_bench[i]->indexReference = indexTree->insert(x, y, i);
    }
    for(int t = 1; t < 20; t++) {
        if (t%5 == 0){
            // build tree
            indexTree = std::make_unique<IndexTree>(SpatialRange(-1.0f,-1.0f,2.0f,2.0f),10);
            for (size_t i = 0; i < balls_bench.size(); i++){
                const GLfloat x = balls_bench[i]->getX(), y = balls_bench[i]->getY();
                balls_bench[i]->indexReference = indexTree->insert(x, y, i);
            }
        }
        powerBall_bench->move();
        for (size_t i = 0; i < balls_bench.size(); i++){
            if (balls_bench[i]->move())
                balls_bench[i]->indexReference = indexTree->insert(balls_bench[i]->getX(), balls_bench[i]->getY(), i);
        }
        
        //seach collide
        using locs =std::vector<size_t>;
        for (size_t i = 0; i < balls_bench.size(); i++)
            if (isColliding(powerBall_bench.get(),balls_bench[i].get())){
                powerBall_bench->collideWith(balls_bench[i].get());
            }
        for (size_t i = 0; i < balls_bench.size(); i++){
            const GLfloat x = balls_bench[i]->getX(), y = balls_bench[i]->getY(),r = balls_bench[i]->getR() * 2;
            const GLfloat minx = std::max(-1.0f,x-r), miny = std::max(-1.0f,y-r);
            const GLfloat maxx = std::min(1.0f,x+r), maxy = std::min(1.0f,y+r);
            SpatialRange range(minx, miny,maxx - minx, maxy - miny);
            locs neibors = indexTree->queryRange(range);
            for (locs::const_iterator it = neibors.begin(); it != neibors.end(); it++){
                if ((*it != i) &&isColliding(balls_bench[*it].get(),balls_bench[i].get())){
                    balls_bench[i]->collideWith(balls_bench[*it].get());
                }
            }
        }
        ballVertices_bench->update();
        powerVertices_bench->update();
    }
}
void updateSpatialByNoIndex(GLfloat gridsize){
    std::vector<std::unique_ptr<BallPara>> balls_bench;
    std::unique_ptr<BallPara> powerBall_bench;
    std::unique_ptr<Ball> ballVertices_bench,powerVertices_bench;
    {
        WindowParas& windowPara = WindowParas::getInstance();
        const GLfloat xgrid = gridsize / windowPara.WINDOW_WIDTH,ygrid = gridsize / windowPara.WINDOW_HEIGHT;
        std::vector<Vertex> normalVert;
        Vertex powerVert;
        for (GLfloat i = -1.0 + xgrid; i < 1.0f - xgrid; i += xgrid)
            for (GLfloat j = - 1.0 + ygrid; j < 1.0f - ygrid; j += ygrid){
                const GLfloat randX = i + (30 + rand()%50)/100 * xgrid,randY = j + (30 + rand()%50)/100 * ygrid;
                const glm::vec3 location = glm::vec3(randX,randY,0);
                if (i < 0 && i + xgrid >= 0 && j <0 && j + ygrid >= 0)
                    powerVert = Vertex(location,color_setting[BallType::power]);
                else
                    normalVert.push_back(Vertex(location,color_setting[BallType::normal]));
            }
        ballVertices_bench = std::make_unique<Ball>(normalVert,radius_setting[BallType::normal],color_setting[BallType::normal]);
        powerVertices_bench = std::make_unique<Ball>(powerVert,radius_setting[BallType::power],color_setting[BallType::power]);
        GLuint counter = 0;
        for (GLfloat i = -1.0 + xgrid; i < 1.0f - xgrid; i += xgrid)
            for (GLfloat j = - 1.0 + ygrid; j < 1.0f - ygrid; j += ygrid){
                if (i < 0 && i + xgrid >= 0 && j <0 && j + ygrid >= 0){
                    powerBall_bench = std::make_unique<BallPara>(powerVertices_bench->getX(0),powerVertices_bench->getY(0),BallType::power);
                    powerBall_bench->setV(Velocity(0.8f, 0.6f));
                }
                else{
                    balls_bench.push_back(std::make_unique<BallPara>(ballVertices_bench->getX(counter),ballVertices_bench->getY(counter),BallType::normal));
                    ++counter;
                }
            }
        
    }
    // build tree
    std::shared_ptr<IndexTree> indexTree = std::make_unique<IndexTree>(SpatialRange(-1.0f,-1.0f,2.0f,2.0f),10);
    for (size_t i = 0; i < balls_bench.size(); i++){
        const GLfloat x = balls_bench[i]->getX(), y = balls_bench[i]->getY();
        balls_bench[i]->indexReference = indexTree->insert(x, y, i);
    }
    for(int t = 1; t < 20; t++) {
        if (t%5 == 0){
            // build tree
            indexTree = std::make_unique<IndexTree>(SpatialRange(-1.0f,-1.0f,2.0f,2.0f),10);
            for (size_t i = 0; i < balls_bench.size(); i++){
                const GLfloat x = balls_bench[i]->getX(), y = balls_bench[i]->getY();
                balls_bench[i]->indexReference = indexTree->insert(x, y, i);
            }
        }
        powerBall_bench->move();
        for (size_t i = 0; i < balls_bench.size(); i++){
            if (balls_bench[i]->move())
                balls_bench[i]->indexReference = indexTree->insert(balls_bench[i]->getX(), balls_bench[i]->getY(), i);
        }
        
        //seach collide
        for (size_t i = 0; i < balls_bench.size(); i++)
            if (isColliding(powerBall_bench.get(),balls_bench[i].get())){
                powerBall_bench->collideWith(balls_bench[i].get());
            }
        for (size_t i = 0; i < balls_bench.size(); i++)
            for (size_t j = 0; j < balls_bench.size(); j++)
                if (i != j && isColliding(balls_bench[i].get(),balls_bench[j].get())){
                    balls_bench[i]->collideWith(balls_bench[j].get());
                    break;
                }
        ballVertices_bench->update();
        powerVertices_bench->update();
    }
}

static void BM_QuadTreeIndex(benchmark::State& state) {
    GLFWwindow *& window = WindowParas::getInstance().window;
    if (!HAS_INIT_OPENGL_CONTEXT && initOpenGL(window,"2025Autumn数据结构实习-粒子碰撞") != 0)
        return;
    InitResource(window);
    for (auto _ : state) {
        updateSpatialByQuadTreeIndex(state.range(0));
    }
}
static void BM_NoIndex(benchmark::State& state) {
    GLFWwindow *& window = WindowParas::getInstance().window;
    if (!HAS_INIT_OPENGL_CONTEXT && initOpenGL(window,"2025Autumn数据结构实习-粒子碰撞") != 0)
        return;
    InitResource(window);
    for (auto _ : state) {
        updateSpatialByNoIndex(state.range(0));
    }
}
BENCHMARK(BM_QuadTreeIndex)->Arg(72)->Arg(52)->Arg(36)->Arg(24)->Arg(18)->Arg(12);
BENCHMARK(BM_NoIndex)->Arg(72)->Arg(52)->Arg(36)->Arg(24)->Arg(18)->Arg(12);

//BENCHMARK_MAIN();

//
//  main.cpp
//  data_structure
//
//  Created by ChanningTong on 10/9/24.
//

#include <iostream>
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <cstring>
#include <string>
#include <memory>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "OpenGL/graphing.hpp"
#include "OpenGL/window.hpp"
#include "applications/component.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int maze_main();
int binarytree_main();
int main(int argc, char **argv){
    if (argc == 1){
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
    std::string program_type = argv[1];
    if (program_type == "test_basicADT"){
        std::cout<<"test"<<std::endl;
        ::testing::InitGoogleTest(&argc, argv);
        ::testing::FLAGS_gtest_filter = "-BinarySearchTreeTest.CopyTest";
        return RUN_ALL_TESTS();
    }else if (program_type == "benchmark"){
        std::cout<<"benchmark"<<std::endl;
    }else if (program_type == "maze"){
        std::cout<<"maze"<<std::endl;
        maze_main();
    }else if (program_type == "binarytree"){
        std::cout<<"binarytree"<<std::endl;
        binarytree_main();
    }
    return 0;
}

int maze_main(){
    using namespace maze;
    GLFWwindow *& window = WindowParas::getInstance().window;
    if (!HAS_INIT_OPENGL_CONTEXT && initOpenGL(window,"2025Autumn数据结构实习-迷宫") != 0)
        return -1;
    InitResource(window);
    initImGUI(window);
    Map map;
    map.generate();
    Boundary* boundary = new Boundary(map.getBoundaryVert());
    Path* path = nullptr;
    if (map.solve())
        path = new Path(map.getPathVert());
    Recorder& record = Recorder::getRecord();
    record.mazeShowUpStartTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        boundary->draw();
        if (path != nullptr){
            path->draw();
            if (record.autoStepping){
                if (!path->autostep())
                    Clear(path,boundary,map);
            }
            else if (record.toStepOver){
                if (!path->Showed())
                    path->StepIn();
                else if (!path->Eliminated())
                    path->StepOut();
                else
                    Clear(path,boundary,map);
                record.toStepOver = false;
            }
        }else if(record.toStepOver || record.autoStepping)
            Clear(path,boundary,map);
        glfwSwapBuffers(window);
    }
    
    delete boundary;
    delete path;
    glfwDestroyWindow(window);
    glfwTerminate();
    WindowParas::getInstance().window = nullptr;
    return 0;
}

int binarytree_main(){
    using namespace binarytree;
    using tcb::SpatialRange;
    GLFWwindow *& window = WindowParas::getInstance().window;
    Recorder& recorder = Recorder::getRecord();
    if (!HAS_INIT_OPENGL_CONTEXT && initOpenGL(window,"2025Autumn数据结构实习-粒子碰撞") != 0)
        return -1;
    InitResource(window);
    initImGUI(window);
    Scatter(balls,recorder.gridsize);
    std::shared_ptr<IndexTree> indexTree = std::make_unique<IndexTree>(SpatialRange(-1.0f,-1.0f,2.0f,2.0f),10);
    BuildIndexTree(indexTree);
    double rebuildingTime = glfwGetTime(),startTIme = glfwGetTime();
    unsigned long long counter = 0;
    //glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window)) {
        if (recorder.toRestart && glfwGetTime() - recorder.restartTime > 1){
            Scatter(balls,recorder.gridsize);
            recorder.toRestart = false;
            indexTree = std::make_unique<IndexTree>(SpatialRange(-1.0f,-1.0f,2.0f,2.0f),10);
            BuildIndexTree(indexTree);
            rebuildingTime = glfwGetTime();
            startTIme = glfwGetTime();
            recorder.stretching = false;
            recorder.startmoving = false;
        }
        glfwPollEvents();
        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (glfwGetTime() - rebuildingTime > 0.2){
            indexTree = std::make_unique<IndexTree>(SpatialRange(-1.0f,-1.0f,2.0f,2.0f),10);
           BuildIndexTree(indexTree);
            rebuildingTime = glfwGetTime();
        }
        if (Recorder::getRecord().startmoving){
            powerBall->move();
            for (size_t i = 0; i < balls.size(); i++){
                if (balls[i]->move())
                    balls[i]->indexReference = indexTree->insert(balls[i]->getX(), balls[i]->getY(), i);
            }
            if (Recorder::getRecord().useSpatialIndex)
                SpatialIndexCollideSeach(indexTree,counter);
            else
                BasicCollideSearch(counter);
        }
        ballVertices->update();
        powerVertices->update();
        double timeticShowUp = glfwGetTime() - startTIme;
        double timeticPopout = glfwGetTime() - recorder.restartTime;
        if (timeticShowUp < 1){
            ballVertices->draw(timeticShowUp);
            powerVertices->draw(timeticShowUp);
        }else if (timeticPopout < 1){
            ballVertices->draw(1 - timeticPopout);
            powerVertices->draw(1 - timeticPopout);
        }else{
            ballVertices->draw();
            powerVertices->draw();
        }
        DrawGUI(counter);
        if (arrow != nullptr)
            arrow->draw();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

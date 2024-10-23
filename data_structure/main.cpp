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
    GLFWwindow *& window = WindowParas::getInstance().window;
    if (!HAS_INIT_OPENGL_CONTEXT && initOpenGL(window,"2025Autumn数据结构实习-粒子碰撞") != 0)
        return -1;
    InitResource(window);
    Scatter(balls,72);
    glfwSwapInterval(2);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (size_t i = 0; i < balls.size(); i++){
            balls[i]->draw();
            balls[i]->move();
        }
        for (size_t i = 0; i < balls.size(); i++)
            for (size_t j = i+1; j < balls.size(); j++){
                if (isColliding(balls[i].get(),balls[j].get())){
                    balls[i]->collideWith(balls[j].get());
                }
            }
        if (arrow != nullptr)
            arrow->draw();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

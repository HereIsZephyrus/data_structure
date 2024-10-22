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
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "OpenGL/graphing.hpp"
#include "OpenGL/window.hpp"
#include "applications/component.hpp"

int maze_main();
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
    }
    return 0;
}

int maze_main(){
    using namespace maze;
    GLFWwindow *& window = WindowParas::getInstance().window;
    if (!HAS_INIT_OPENGL_CONTEXT && initOpenGL(window,"2025Autumn数据结构实习-迷宫") != 0)
        return -1;
    InitResource();
    Map map;
    map.generate();
    Primitive* boundary = new Primitive(map.getBoundaryVert(), GL_LINES,ShaderBucket["inside"].get());
    Path* path = nullptr;
    if (map.solve())
        path = new Path(map.getPathVert());
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        boundary->draw();
        if (path != nullptr)
            path->draw();
        if (Recorder::getRecord().autoStepping)
            map.autostep();
        else if (Recorder::getRecord().toStepOver){
            if (!path->Finished())
                path->StepIn();
            else{
                map.clear();
                map.generate();
                map.solve();
                delete boundary;
                delete path;
                boundary = new Primitive(map.getBoundaryVert(), GL_LINES,ShaderBucket["inside"].get());
                path = new Path(map.getPathVert());
            }
            Recorder::getRecord().toStepOver = false;
        }
        
        glfwSwapBuffers(window);
    }
    
    delete boundary;
    delete path;
    glfwDestroyWindow(window);
    glfwTerminate();
    WindowParas::getInstance().window = nullptr;
    return 0;
}

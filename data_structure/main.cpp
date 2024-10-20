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
#include "OpenGL/environment.hpp"
#include "OpenGL/window.hpp"

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
    GLFWwindow *& window = WindowParas::getInstance().window;
    if (!HAS_INIT_OPENGL_CONTEXT && initOpenGL(window,"2025Autumn数据结构实习-迷宫") != 0)
        return -1;
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glfwSwapBuffers(window);
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    WindowParas::getInstance().window = nullptr;
    return 0;
}

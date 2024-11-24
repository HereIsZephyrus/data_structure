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
#include "dictionary/dictionary.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "gdal.h"

int maze_main();
int binarytree_main();
int transport_main();
int dictionary_main();
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
        ::benchmark::Initialize(&argc, argv);
        ::benchmark::RunSpecifiedBenchmarks();
        std::cout<<"benchmark"<<std::endl;
    }else if (program_type == "maze"){
        std::cout<<"maze"<<std::endl;
        maze_main();
    }else if (program_type == "binarytree"){
        std::cout<<"binarytree"<<std::endl;
        binarytree_main();
    }else if (program_type == "transport"){
        std::cout<<"transport"<<std::endl;
        transport_main();
    }else if (program_type == "dictionary"){
        std::cout<<"dictionary"<<std::endl;
        dictionary_main();
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
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

int transport_main(){
    using namespace transport;
    GLFWwindow *& window = WindowParas::getInstance().window;
    Recorder& recorder = Recorder::getRecord();
    if (!HAS_INIT_OPENGL_CONTEXT && initOpenGL(window,"2025Autumn数据结构实习-高铁最短路") != 0)
        return -1;
    InitResource(window);
    GDALAllRegister();
    std::string geojsonFolder = "/Users/channingtong/Program/data_structure/data_structure/GeoResource/";
    std::string cityPath = geojsonFolder + "city.geojson";
    std::string trunkPath = geojsonFolder + "trunk.geojson";
    vector<vector<Vertex>> trunkPoints;
    vector<Vertex> cityPoints;
    vector<Station> stations;
    loadLineGeoJsonResource(trunkPoints,trunkPath,recorder.defaultTrunkColor);
    loadPointGeoJsonResource(cityPoints,stations,cityPath,recorder.defaultCityColor);
    vector<std::unique_ptr<Trunk>> trunks;
    for (vector<vector<Vertex>>::iterator trunkPoint = trunkPoints.begin(); trunkPoint != trunkPoints.end(); trunkPoint++){
        trunks.push_back(std::make_unique<Trunk>(*trunkPoint));
    }
    Citys citygroup = Citys(cityPoints);
    calcDirectLength(stations,citygroup);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (vector<std::unique_ptr<Trunk>>::iterator trunk = trunks.begin(); trunk != trunks.end(); trunk++)
            (*trunk)->draw();
        if (recorder.toCheckSelect){
            recorder.toCheckSelect = false;
            bool toSolve = citygroup.checkSelect(recorder.clickLoc);
            if (toSolve)
                InitSolvers(recorder.startID,recorder.endID,stations);
        }
        citygroup.draw();
        if (recorder.toGenerateRoute){
            bool toMoveStep = checkWholeTic();
            if (toMoveStep)
                ++recorder.tickStep;
            recorder.dfsSolver->checkToSolve(recorder.tickStep, stations);
            recorder.dfsPath->draw();
            recorder.primSolver->checkToSolve(recorder.tickStep, stations);
            recorder.primPath->draw();
        }
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return  0;
}
int dictionary_main(){
    using namespace dict;
    //BSTDictionary bstdict;
    AVLDictionary avldict;
    std::vector<DictItem> rawItems;
    readDictfile("/Users/channingtong/Program/data_structure/data_structure/dictionary/EnWords.csv", rawItems);
    //bstdict.dm.readDictList(rawItems);
    avldict.dm.readDictList(rawItems);
    GLFWwindow *& window = WindowParas::getInstance().window;
    if (!HAS_INIT_OPENGL_CONTEXT && initOpenGL(window,"2025Autumn数据结构实习-字典") != 0)
        return -1;
    initImGUI(window);
    Recorder& recorder = Recorder::getRecord();
    recorder.ChangeImGUIStyle();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(1.0,1.0,1.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        Recorder& recorder = Recorder::getRecord();
        WindowParas& windowPara = WindowParas::getInstance();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(windowPara.SIDEBAR_WIDTH, windowPara.WINDOW_HEIGHT));
        
        ImGui::Begin("Do you mean:", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        if (!recorder.items.empty()){
            ImGui::PushFont(recorder.englishFont);
            const int size = static_cast<int>(recorder.items.size());
            const char** items = new const char*[size];
            int i = 0;
            for (std::vector<string>::const_iterator item = recorder.items.begin(); item != recorder.items.end(); item++,i++)
                items[i] = item->c_str();
            ImGui::ListBox("##", &recorder.selectedItemIndex, items, size,20);
            recorder.recordWord = items[recorder.selectedItemIndex];
            delete[] items;
            ImGui::PopFont();
        }
        ImGui::End();
        ImGui::SetNextWindowPos(ImVec2(windowPara.SIDEBAR_WIDTH, 0));
        ImGui::SetNextWindowSize(ImVec2(windowPara.WINDOW_WIDTH - windowPara.SIDEBAR_WIDTH, windowPara.WINDOW_HEIGHT));
        ImGui::Begin("Dicitionary", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        char searchBuffer[128] = "";
        strcpy(searchBuffer, recorder.recordSearch.c_str());
        ImGui::PushFont(recorder.bandFont);
        ImGui::Text("\uf002");
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::PushFont(recorder.chineseFont);
        ImGui::InputText("单词检索", searchBuffer, sizeof(searchBuffer));
        if (recorder.items.empty())
            ImGui::Text("%s", "no such word");
        else
            //ImGui::Text("%s", bstdict.query(recorder.recordWord).c_str());
            ImGui::Text("%s", avldict.query(recorder.recordWord).c_str());
        //std::cout<<searchBuffer<<std::endl;
        if (string(searchBuffer) != recorder.recordSearch){
            recorder.recordSearch = searchBuffer;
            recorder.selectedItemIndex = 0;
            //bstdict.querysubtree(recorder.recordSearch,recorder.items);
            avldict.querysubtree(recorder.recordSearch,recorder.items);
        }
        ImGui::PopFont();
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

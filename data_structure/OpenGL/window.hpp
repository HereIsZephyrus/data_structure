//
//  window.hpp
//  data_structure
//
//  Created by ChanningTong on 10/21/24.
//

#ifndef window_hpp
#define window_hpp

#include <cstring>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

inline bool HAS_INIT_OPENGL_CONTEXT = false;
int initOpenGL(GLFWwindow *&window,std::string windowName);
class WindowParas{
public:
    static WindowParas& getInstance(){
        static WindowParas instance;
        return instance;
    }
    WindowParas(const WindowParas&) = delete;
    void operator=(const WindowParas&) = delete;
    GLFWwindow * window;
    const GLint WINDOW_WIDTH = 960;
    const GLint WINDOW_HEIGHT = 720;
    const GLint SIDEBAR_WIDTH = 300;
    GLint SCREEN_WIDTH,SCREEN_HEIGHT;
    GLfloat xScale,yScale;
    GLfloat defaultAlpha;
    bool mainWindowFocused;
    GLfloat screen2normalX(GLdouble screenX);
    GLfloat screen2normalY(GLdouble screenY);
    GLfloat normal2orthoX(GLfloat normalX);
    GLfloat normal2orthoY(GLfloat normalY);
private:
    WindowParas() {}
};

namespace gui {
constexpr GLfloat detactBias = 0.48f;
constexpr GLfloat outboundBias = 2.0f;
}
#endif /* window_hpp */

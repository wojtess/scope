#ifndef GUI_H
#define GUI_H

#include <GL/glew.h>
#include <string>
#include <thread>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <atomic>
#include "shader.h"

class gui
{
private:
    std::atomic_bool _running = { true };
    std::atomic<GLFWwindow*> _window;
    std::atomic_int _fps;
    Shader _glShader;

    GLuint VBO;

    void render();
    void cleanUp();
public:
    gui(/* args */);
    ~gui();
    int begin(int width, int height, std::string name, int fps);
    std::thread startThread();
};
#endif
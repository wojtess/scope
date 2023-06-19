// #ifndef GUI_H
// #define GUI_H
#pragma once
#include <GL/glew.h>
#include <string>
#include <thread>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <atomic>
#include <vector>
#include <mutex>
#include "dataSupply.h"

namespace gui {

    struct state {
        bool running = { true };
        GLFWwindow* window;
        int fps;
        supply::data supply;
    };

    class window
    {
    private:
        void render();
        void cleanUp();
    public:
        std::mutex stateMutex;
        gui::state state;

        window(/* args */);
        ~window();
        int begin(int width, int height, std::string name, int fps);
        std::thread startThread();
    };
}

// #endif
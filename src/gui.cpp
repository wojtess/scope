#include "gui.h"
#include "implot.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <memory>
#include <cmath>
#include <cstdlib>

gui::window::window() {
    innerState.running = true;
}

gui::window::~window() {
    this->innerState.running = false;
}

int gui::window::begin(int width, int height, std::string name, int fps) {
    // GL 3.0 + GLSL 130
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create window with graphics context
    this->innerState.window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (this->innerState.window == NULL)
        return 1;
    glfwMaximizeWindow(this->innerState.window);
    
    glfwMakeContextCurrent(this->innerState.window);

    int glewStatus = glewInit();
    if ( glewStatus != GLEW_OK ) {
        printf("glew: %s\n", glewGetErrorString(glewStatus));
        return 2;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    if(fps <= 0) {
        glfwSwapInterval(1); // Enable vsync
    } else {
        glfwSwapInterval(0); // Disable vsync
    }
    this->innerState.fps = fps;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(this->innerState.window, true);
    return 0;
}

float t = 0.0f;

void gui::window::render() {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //RENDERING IMGUI

    ImGui::ShowDemoWindow();

    ImGui::Begin("settings");
        int buffer_size = innerState.supply.buf.getSize();
        ImGui::DragInt("buffer", &buffer_size, 5.0f, 10, 10000);
        innerState.supply.buf.setSize(buffer_size);
        
        const char* current_item = innerState.supply.port.c_str();
        if(ImGui::BeginCombo("port", current_item)) {
            auto ports = innerState.supply.ports;
            for (int i = 0; i < ports.size(); i++) {
                auto port = ports[i];
                bool is_selected = (innerState.supply.port.compare(port) == 0);
                const char* c_str_port = port.c_str();
                if(ImGui::Selectable(c_str_port, is_selected)) {
                    innerState.supply.port = port;
                }
                if(is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            
            ImGui::EndCombo();
        }
        innerState.supply.port = std::string(current_item);
    ImGui::End();

    ImGui::Begin("plot");
        auto values = innerState.supply.getValues();
        float* xValues = new float[values.size()];
        float* yValues = new float[values.size()];
        for (int i = 0; i < values.size(); i++) {
            auto value = values[i];
            xValues[i] = value.value;
            yValues[i] = value.timestamp;
        }
        if(ImPlot::BeginPlot("plot", ImVec2(-1,-1))) {
            ImPlot::SetupAxes(NULL, NULL);
            ImPlot::SetupAxisLimits(ImAxis_X1, yValues[0], yValues[0] + values.size(), ImGuiCond_Always);
            ImPlot::PlotLine("", yValues, xValues, values.size());
            ImPlot::EndPlot();
        }
        delete xValues;
        delete yValues;
    ImGui::End();

    //END RENDERING IMGUI

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(this->innerState.window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(this->innerState.window);
}

void gui::window::cleanUp() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    ImPlot::DestroyContext();

    glfwDestroyWindow(this->innerState.window);
    glfwTerminate();

    exit(0);
}

std::thread gui::window::startThread() {
    auto thread = std::thread([this]() {

        double lastFrameTime = 0;   // number of seconds since the last frame

        while(this->innerState.running) {
            //check for state mutex and change data
            if(memcmp(&this->innerState, &this->prevInnerState, sizeof(this->innerState)) != 0) {
                //change in innerState
                if(this->stateMutex.try_lock()) {
                    //lock succesful
                    //cahnge public state
                    this->state = this->innerState;
                    this->stateMutex.unlock();
                    this->prevInnerState = this->innerState;
                } else {
                    //lock unsuccesful
                    //dont change anythink, maybe next time mutex can be locked, then save state
                }
            }
            

            //rendering stuff
            if(glfwWindowShouldClose(this->innerState.window)) {
                    this->innerState.running = false;
            }

            if(this->innerState.fps <= 0) {
                glfwMakeContextCurrent(this->innerState.window);
                this->render();
            } else {
                double now = glfwGetTime();

                if ((now - lastFrameTime) >= 1.0 / this->innerState.fps) {
                    lastFrameTime = now;

                    glfwMakeContextCurrent(this->innerState.window);
                    this->render();
                }
            }
            
        }
        this->cleanUp();
    });
    thread.detach();
    return thread;
}
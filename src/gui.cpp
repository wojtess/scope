#include "gui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <GL/glew.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <memory>

const std::string vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}";

const std::string fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n";

gui::gui() {
    
}

gui::~gui() {
    this->_running = false;
}

int gui::begin(int width, int height, std::string name, int fps) {
    // GL 3.0 + GLSL 130
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create window with graphics context
    this->_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (this->_window == NULL)
        return 1;
    glfwMaximizeWindow(_window);
    
    glfwMakeContextCurrent(_window);

    int glewStatus = glewInit();
    if ( glewStatus != GLEW_OK ) {
        printf("glew: %s\n", glewGetErrorString(glewStatus));
        return 2;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    if(fps <= 0) {
        glfwSwapInterval(1); // Enable vsync
    } else {
        glfwSwapInterval(0); // Disable vsync
    }
    this->_fps = fps;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(this->_window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    this->_glShader.begin(vertexShaderSource, fragmentShaderSource);

    // glGenBuffers(1, &this->VBO);
    // glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);


    // bind the VAO (it was already bound, but just to demonstrate): seeing as we only have a single VAO we can 
    // just bind it beforehand before rendering the respective triangle; this is another approach.
    glBindVertexArray(VAO);

    return 0;
}

void gui::render() {
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

    ImGui::Begin("name");
        ImGui::Text("");
    ImGui::End();

    //END RENDERING IMGUI

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);


    this->_glShader.use();

    int vertexColorLocation = glGetUniformLocation(this->_glShader.getProgramID(), "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);

    float vertices[] = {
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f   // top 
    };

    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render the triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(_window);
}

void gui::cleanUp() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(this->_window);
    glfwTerminate();
}

std::thread gui::startThread() {
    printf("STARTING THREAD\n");
    auto thread = std::thread([this]() {

        double lastFrameTime = 0;   // number of seconds since the last frame

        while(this->_running) {
            
            if(glfwWindowShouldClose(this->_window)) {
                    this->_running = false;
            }

            if(this->_fps <= 0) {
                glfwMakeContextCurrent(this->_window);
                this->render();
            } else {
                double now = glfwGetTime();

                if ((now - lastFrameTime) >= 1.0 / this->_fps) {
                    lastFrameTime = now;

                    glfwMakeContextCurrent(this->_window);
                    this->render();
                }
            }
            
        }
        printf("CLEAN UP\n");
        this->cleanUp();
    });
    thread.detach();
    return thread;
}
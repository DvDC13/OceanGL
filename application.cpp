#include "application.h"

namespace Application
{

    Camera camera_(0.0f, 250.0f, 800.0f, 0.0f, 1.0f, 0.0f, -90.0f, 0.0f);
    float lastX = 0.0f;
    float lastY = 0.0f;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    bool firstMouse = true;

    void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    Application::Application()
    {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            fprintf(stderr, "Failed to initialize GLFW\n");

        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

        // Create window with graphics context
        window_ = glfwCreateWindow(m_windowWidth, m_windowHeight, "OceanGL Simulation", nullptr, nullptr);
        if (window_ == nullptr)
            fprintf(stderr, "Failed to create GLFW window\n");
        glfwMakeContextCurrent(window_);
        glfwSwapInterval(1); // Enable vsync

        // Initialize OpenGL loader
        bool err = glewInit() != GLEW_OK;
        if (err)
        {
            fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        }

        glfwSetCursorPosCallback(window_, mouse_callback);
        glfwSetScrollCallback(window_, scroll_callback);

        glEnable(GL_DEPTH_TEST);

        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << " initialized on " << glGetString(GL_VENDOR) << " " << glGetString(GL_RENDERER)
                << " using GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window_, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // Variables initialization
        lastX = m_windowWidth / 2.0f;
        lastY = m_windowHeight / 2.0f;
    }

    Application::~Application()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window_);
        glfwTerminate();
    }

    void Application::Run()
    {
        Start();

        while (!glfwWindowShouldClose(window_))
        {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            time = glfwGetTime();

            glfwPollEvents();

            int display_w, display_h;
            glfwGetFramebufferSize(window_, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color_.x * clear_color_.w, clear_color_.y * clear_color_.w, clear_color_.z * clear_color_.w, clear_color_.w);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            Update();

            // Rendering
            ImGui::Render();

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window_);
        }
    }

    void Application::Start()
    {
        std::string vertex_shader_path = "/home/david/Desktop/Image/POGLA/PROJET/OceanGL/shaders/vertex_shader.glsl";
        std::string fragment_shader_path = "/home/david/Desktop/Image/POGLA/PROJET/OceanGL/shaders/fragment_shader.glsl";
        std::string tessellation_control_shader_path = "/home/david/Desktop/Image/POGLA/PROJET/OceanGL/shaders/tess_control.glsl";
        std::string tessellation_evaluation_shader_path = "/home/david/Desktop/Image/POGLA/PROJET/OceanGL/shaders/tess_eval.glsl";

        program_ = MyGL::Program::make_program(vertex_shader_path, fragment_shader_path, tessellation_control_shader_path, tessellation_evaluation_shader_path);

        if (!program_)
        {
            std::cerr << "Failed to create program !" << std::endl;
            return;
        }

        program_->use();

        // create a larger plane
        vertices_positions_ = {
            -1.0f, 0.0f, -1.0f, // bottom left
            1.0f, 0.0f, -1.0f, // bottom right
            1.0f, 0.0f, 1.0f, // top right
            -1.0f, 0.0f, 1.0f  // top left
        };

        vao_ = new MyGL::Vao();
        vao_->bind();

        MyGL::Vbo vbo;
        vbo.bind();

        vbo.setData(vertices_positions_.data(), vertices_positions_.size() * sizeof(float), GL_STATIC_DRAW);

        MyGL::Vbl vbl;
        vbl.push<float>(3);

        vao_->addBuffer(vbo, vbl);

        vbo.unbind();
        vao_->unbind();

        program_->unuse();

        numberOfWaves = 40;
        roughness = 0.5f;
        metallic = 0.5f;

        amplitude = 0.05f;
        frequency = 3.0f;
        amplitude_attenuation = 0.82f;
        frequency_amplification = 1.18f;

        lightPos = glm::vec3(0.0f, 10.0f, 0.0f);
        lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        lightIntensity = 1.0f;

        oceanColor = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    void Application::Update()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                if (ImGui::MenuItem("Exit"))
                {
                    glfwSetWindowShouldClose(window_, true);
                }

                ImGui::EndMenu();
            }

            ImGui::Text("   Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::EndMainMenuBar();
        }

        ImGui::Begin("Ocean Parameters");

        ImGui::SliderInt("Number of waves", &numberOfWaves, 0, 100);
        ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);
        ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f);
        ImGui::SliderFloat("Amplitude", &amplitude, 0.0f, 1.0f);
        ImGui::SliderFloat("Frequency", &frequency, 0.0f, 100.0f);
        ImGui::SliderFloat("Amplitude attenuation", &amplitude_attenuation, 0.0f, 1.0f);
        ImGui::SliderFloat("Frequency amplification", &frequency_amplification, 1.0f, 3.0f);

        ImGui::ColorEdit3("Ocean color", (float*)&oceanColor);

        ImGui::End();

        ImGui::Begin("Light Parameters");

        ImGui::SliderFloat3("Light position", &lightPos.x, -100.0f, 100.0f);
        ImGui::SliderFloat3("Light color", &lightColor.x, 0.0f, 1.0f);
        ImGui::SliderFloat("Light intensity", &lightIntensity, 0.0f, 10.0f);

        ImGui::End();

        program_->use();

        program_->setUniform1f("time", time);
        program_->setUniform1i("numberOfWaves", numberOfWaves);
        program_->setUniformVec3f("cameraPosition", camera_.Position.x, camera_.Position.y, camera_.Position.z);
        program_->setUniform1f("roughness", roughness);
        program_->setUniform1f("metallic", metallic);
        program_->setUniform1f("amplitude", amplitude);
        program_->setUniform1f("frequency", frequency);
        program_->setUniform1f("amplitude_attenuation", amplitude_attenuation);
        program_->setUniform1f("frequency_amplification", frequency_amplification);
        program_->setUniformVec3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
        program_->setUniformVec3f("lightColor", lightColor.x, lightColor.y, lightColor.z);
        program_->setUniform1f("lightIntensity", lightIntensity);
        program_->setUniformVec3f("oceanColor", oceanColor.x, oceanColor.y, oceanColor.z);

        glm::mat4 view = camera_.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera_.Zoom, (float)m_windowWidth / (float)m_windowHeight, 0.1f, 1000.0f);

        program_->setUniformMat4f("view", view);
        program_->setUniformMat4f("projection", projection);

        vao_->bind();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1000.0f, 1000.0f, 1000.0f));
        program_->setUniformMat4f("model", model);

        glDrawArrays(GL_PATCHES, 0, 4); CHECK_GL_ERROR();
        glPatchParameteri(GL_PATCH_VERTICES, 4); CHECK_GL_ERROR();

        vao_->unbind();

        program_->unuse();
    }

    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            if (firstMouse)
            {
                lastX = xpos;
            lastY = ypos;
                firstMouse = false;
            }

            GLfloat xoffset = xpos - lastX;
            GLfloat yoffset = lastY - ypos;

            lastX = xpos;
            lastY = ypos;

            camera_.ProcessMouseMovement(xoffset, yoffset);
        }
        else
        {
            firstMouse = true;
        }
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        camera_.ProcessMouseScroll(yoffset);
    }
}
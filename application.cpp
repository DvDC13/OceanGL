#include "application.h"

namespace Application
{
    Camera camera_(0.0f, 0.4f, 2.5f, 0.0f, 1.0f, 0.0f, -90.0f, 0.0f);

    float lastX = 0.0f;
    float lastY = 0.0f;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    bool firstMouse = true;

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
        std::string vertex_shader_path = "../shaders/ocean_vertex_shader.glsl";
        std::string fragment_shader_path = "../shaders/ocean_fragment_shader.glsl";
        std::string tessellation_control_shader_path = "../shaders/ocean_tess_control.glsl";
        std::string tessellation_evaluation_shader_path = "../shaders/ocean_tess_eval.glsl";

        program_ = MyGL::Program::make_program(vertex_shader_path, fragment_shader_path, tessellation_control_shader_path, tessellation_evaluation_shader_path);

        if (!program_)
        {
            std::cerr << "Failed to create program !" << std::endl;
            return;
        }

        std::string skybox_vertex_shader_path = "../shaders/skybox_shader_vert.glsl";
        std::string skybox_fragment_shader_path = "../shaders/skybox_shader_frag.glsl"; 
        std::string skybox_tessellation_control_shader_path = "../shaders/skybox_shader_tess_control.glsl";
        std::string skybox_tessellation_evaluation_shader_path = "../shaders/skybox_shader_tess_eval.glsl";
        skybox_program_ = MyGL::Program::make_program(skybox_vertex_shader_path, skybox_fragment_shader_path, skybox_tessellation_control_shader_path, skybox_tessellation_evaluation_shader_path);

        if (!skybox_program_)
        {
            std::cerr << "Failed to create skybox program !" << std::endl;
            return;
        }

        skybox_vao_ = new MyGL::Vao();
        skybox_vao_->bind();
        skybox_vao_->unbind();
        skybox_program_->unuse();

        program_->use();

        scaleX = 1;
        scaleZ = 1;

        // create a larger plane
        vertices_positions_ = {
            -1.0f * scaleX, 0.0f, -1.0f * scaleZ, // bottom left
            1.0f * scaleX, 0.0f, -1.0f * scaleZ, // bottom right
            1.0f * scaleX, 0.0f, 1.0f * scaleZ, // top right
            -1.0f * scaleX, 0.0f, 1.0f * scaleZ  // top left
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

        amplitude = 0.05f;
        frequency = 3.0f;
        amplitude_attenuation = 0.82f;
        frequency_amplification = 1.18f;
        epsilon = 0.1f;
        seed = 1.0f;

        sunDirection = glm::vec3(3.0f, 2.0f, -5.0f);

        shallowColor = glm::vec3(0.0, 0.25, 0.3);
        deepColor = glm::vec3(0.0, 0.05, 0.2);
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

        ImGui::SliderInt("Number of waves", &numberOfWaves, 0, 50);
        ImGui::SliderFloat("Amplitude", &amplitude, 0.0f, 0.5f);
        ImGui::SliderFloat("Frequency", &frequency, 0.0f, 10.0f);
        ImGui::SliderFloat("Amplitude attenuation", &amplitude_attenuation, 0.0f, 1.0f);
        ImGui::SliderFloat("Frequency amplification", &frequency_amplification, 1.0f, 2.0f);
        ImGui::SliderFloat("Epsilon", &epsilon, 0.001f, 0.5f);
        ImGui::SliderFloat("Seed", &seed, 0.0f, 7.0f);
        ImGui::SliderFloat3("Sun direction", &sunDirection.x, -10.0f, 10.0f);
        ImGui::SliderFloat3("Shallow color", &shallowColor.x, 0.0f, 1.0f);
        ImGui::SliderFloat3("Deep color", &deepColor.x, 0.0f, 1.0f);

        ImGui::Separator();

        ImGui::SliderInt("Scale X", &scaleX, 1, 10);
        ImGui::SliderInt("Scale Z", &scaleZ, 1, 10);

        ImGui::Checkbox("Switch View", &nearView);

        ImGui::End();

        if (nearView)
        {
            camera_.Position = glm::vec3(0.0f, 0.1f, 0.2f);
            camera_.Yaw = -90.0f;
            camera_.Pitch = 0.0f;
        }
        else
        {
            camera_.Position = glm::vec3(0.0f, 0.4f, 2.5f);
            camera_.Yaw = -90.0f;
            camera_.Pitch = 0.0f;
        }

        glm::mat4 view = camera_.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera_.Zoom, (float)m_windowWidth / (float)m_windowHeight, 0.1f, 1000.0f);

        glDepthFunc(GL_LEQUAL);
        skybox_program_->use();
        skybox_vao_->bind();
        skybox_program_->setUniformVec3f("cameraPosition", camera_.Position.x, camera_.Position.y, camera_.Position.z);
        skybox_program_->setUniformMat4f("view", view);
        skybox_program_->setUniformMat4f("projection", projection);
        skybox_program_->setUniformVec3f("sunDirection", sunDirection.x, sunDirection.y, sunDirection.z);
        glPatchParameteri(GL_PATCH_VERTICES, 4); CHECK_GL_ERROR();
        glDrawArrays(GL_PATCHES, 0, 4); CHECK_GL_ERROR();
        skybox_vao_->unbind();
        skybox_program_->unuse();
        glDepthFunc(GL_LESS);

        program_->use();

        program_->setUniform1f("time", time);
        program_->setUniform1i("numberOfWaves", numberOfWaves);
        program_->setUniformVec3f("cameraPosition", camera_.Position.x, camera_.Position.y, camera_.Position.z);
        program_->setUniform1f("amplitude", amplitude);
        program_->setUniform1f("frequency", frequency);
        program_->setUniform1f("amplitude_attenuation", amplitude_attenuation);
        program_->setUniform1f("frequency_amplification", frequency_amplification);
        program_->setUniform1f("epsilon", epsilon);
        program_->setUniform1f("seed", seed);
        program_->setUniformVec3f("sunDirection", sunDirection.x, sunDirection.y, sunDirection.z);
        program_->setUniformVec3f("shallowColor", shallowColor.x, shallowColor.y, shallowColor.z);
        program_->setUniformVec3f("deepColor", deepColor.x, deepColor.y, deepColor.z);

        program_->setUniformMat4f("view", view);
        program_->setUniformMat4f("projection", projection);

        vao_->bind();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(scaleX, 1.0f, scaleZ));
        program_->setUniformMat4f("model", model);

        glPatchParameteri(GL_PATCH_VERTICES, 4); CHECK_GL_ERROR();
        glDrawArrays(GL_PATCHES, 0, 4); CHECK_GL_ERROR();

        vao_->unbind();

        program_->unuse();
    }
}
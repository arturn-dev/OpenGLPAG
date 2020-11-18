// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "OpenGLCtx.h"
#include "SolarSystem.h"
#include <stdio.h>
#include <fstream>

// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually. 
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

bool init = true;

const float mouseSensitivity = 0.1f;
const float scrollSensitivity = 6.0f;
float cursorLastX = 0;
float cursorLastY = 0;
float cursorDeltaX = 0;
float cursorDeltaY = 0;
float scrollDelta = 0;

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	const auto xPosf = static_cast<float>(xPos);
	const auto yPosf = static_cast<float>(yPos);
	
	if (init)
	{
		init = false;
		cursorLastX = xPosf;
		cursorLastY = yPosf;
	}
	
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
	    cursorDeltaX = xPosf - cursorLastX;
		cursorDeltaY = yPosf - cursorLastY;
    }
    else
    {
	    cursorDeltaX = 0;
    	cursorDeltaY = 0;
    }

	cursorLastX = xPosf;
	cursorLastY = yPosf;
}

void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	scrollDelta = static_cast<float>(yoffset);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 4.3 + GLSL 430
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	int windowW = 1280;
	int windowH = 720;
	
    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(windowW, windowH, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
	glfwSetCursorPosCallback(window, mouseCallback);
	

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    bool show_demo_window = false;

	// GUI controls settings //

	int cylinderRes = 3;
	const int minCylinderRes = 3;
	const int maxCylinderRes = 30;
	bool isWireframeMode = false;

	// End of GUI controls settings //
	
    OpenGLCtx openGlCtx;
	
	try
	{		
		openGlCtx.init();
	}
	catch (std::exception& e)
	{
		printf(e.what());
		return 1;
	}

	SolarSystem solarSystem(openGlCtx.getShaderProgram(), minCylinderRes, maxCylinderRes);

    glm::vec3 xRotationVec(1.0f, 0.0f, 0.0f);
    glm::vec3 yRotationVec(0.0f, 1.0f, 0.0f);
	glm::vec4 userColor(0.0f, 0.0f, 0.0f, 0.0f);

	
	glfwSetScrollCallback(window, mouseScrollCallback);
	
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            ImGui::Begin("Solar System");
        	ImGui::Checkbox("Wireframe mode", &isWireframeMode);

        	ImGui::SliderInt("Cylinder resolution", &cylinderRes, minCylinderRes, maxCylinderRes);
        	ImGui::ColorEdit4("Color", glm::value_ptr(userColor));
        	
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);

    	solarSystem.modelMat.rotate(glm::radians(cursorDeltaX * mouseSensitivity), yRotationVec);
    	solarSystem.modelMat.rotate(glm::radians(cursorDeltaY * mouseSensitivity), xRotationVec);
    	glm::mat4 rotationMat = glm::inverse(solarSystem.modelMat.getTMat()) * glm::inverse(openGlCtx.getViewMat());

    	glm::vec4 xRotationVec4 = rotationMat * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec4 yRotationVec4 = rotationMat * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    	
        xRotationVec = glm::normalize(glm::vec3(xRotationVec4.x, xRotationVec4.y, xRotationVec4.z));
        yRotationVec = glm::normalize(glm::vec3(yRotationVec4.x, yRotationVec4.y, yRotationVec4.z));

    	openGlCtx.setViewMat(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, scrollDelta * scrollSensitivity)) * openGlCtx.getViewMat());

    	solarSystem.setCylinderRes(cylinderRes);
    	solarSystem.animate();
    	
    	openGlCtx.setWireframeMode(isWireframeMode);
    	openGlCtx.render(display_w, display_h, &solarSystem);
    	    	
        cursorDeltaX = 0;
        cursorDeltaY = 0;
    	scrollDelta = 0;
    	
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    openGlCtx.deleteCtx();
	
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

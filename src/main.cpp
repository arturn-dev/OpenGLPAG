// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "OpenGLCtx.h"
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


#include "AssimpModelLoader.h"
#include "Model.h"


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
float cameraSpeedMult = 1.0f;

float timeDelta = 0.0;
double timeLastFrame = 0.0;

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
	cameraSpeedMult += static_cast<float>(yoffset);
	if (cameraSpeedMult <= 0.1f)
        cameraSpeedMult = 0.1f;
}

void processKbInput(GLFWwindow* window, OpenGLCtx& openGlCtx)
{
	auto moveIncrement = 2.5f * timeDelta * cameraSpeedMult;
    auto ifPressed = [window](int key)->bool { return glfwGetKey(window, key) == GLFW_PRESS; };
	
	if (ifPressed(GLFW_KEY_W))
        openGlCtx.getCamera().moveFB(moveIncrement);
	if (ifPressed(GLFW_KEY_S))
        openGlCtx.getCamera().moveFB(-moveIncrement);
	if (ifPressed(GLFW_KEY_A))
        openGlCtx.getCamera().moveLR(moveIncrement);
	if (ifPressed(GLFW_KEY_D))
        openGlCtx.getCamera().moveLR(-moveIncrement);
	if (ifPressed(GLFW_KEY_SPACE))
        openGlCtx.getCamera().moveUD(moveIncrement);
	if (ifPressed(GLFW_KEY_LEFT_SHIFT))
        openGlCtx.getCamera().moveUD(-moveIncrement);
}

void setCameraRotation(OpenGLCtx& openGlCtx)
{
	float pitchDeg = -cursorDeltaY * mouseSensitivity;
	float yawDeg = cursorDeltaX * mouseSensitivity;

	openGlCtx.getCamera().rotate(pitchDeg, yawDeg);
}

std::vector<std::unique_ptr<Object3D>> prepareScene(OpenGLCtx& openGlCtx)
{
	std::vector<std::unique_ptr<Object3D>> objects;
	
	// Prepare shaders
	
	Shader basicVert(".\\res\\shaders\\basic.vert", GL_VERTEX_SHADER);
	Shader basicFrag(".\\res\\shaders\\basic.frag", GL_FRAGMENT_SHADER);
	Shader lightFrag(".\\res\\shaders\\light.frag", GL_FRAGMENT_SHADER);
	//Shader viewVert(".\\res\\shaders\\view.vert", GL_VERTEX_SHADER);
	//Shader viewFrag(".\\res\\shaders\\view.frag", GL_FRAGMENT_SHADER);
	
	basicVert.compileShader();
	basicFrag.compileShader();
	lightFrag.compileShader();
	//viewVert.compileShader();
    //viewFrag.compileShader();

	ShaderProgram basicShader;
	basicShader.attachShader(basicVert);
	basicShader.attachShader(basicFrag);
	basicShader.makeProgram();
	
	
	//

    ShaderProgram lightShader;
	lightShader.attachShader(basicVert);
	lightShader.attachShader(lightFrag);
	lightShader.makeProgram();

	ShaderProgram viewShader;
	//viewShader.attachShader(viewVert);
	//viewShader.attachShader(viewFrag);
	//viewShader.makeProgram();

    auto spPtr = openGlCtx.addShaderProgram(std::move(basicShader));
	auto sp2Ptr = openGlCtx.addShaderProgram(std::move(lightShader));
	//auto sp3Ptr = openGlCtx.addShaderProgram(std::move(viewShader));
	
	// Load and setup models

	AssimpModelLoader<TexMesh> modelLoader(".\\res\\models", ".\\res\\textures");
	Model<TexMesh> cubeObj = modelLoader.loadModel("cube.obj", *spPtr, aiColor4D{0.1f, 0.1f, 0.1f, 1.0f});
	cubeObj.getMeshes()[0].addTexture(OpenGLRender::Texture{OpenGLRender::Texture::TexDiff, modelLoader.getTexturePath("stone.jpg")});
	auto floorObj = cubeObj;
	floorObj.modelMat.scale(glm::vec3(100.0f, 1.0f, 100.0f));
	cubeObj.modelMat.scale(glm::vec3(10.0f, 1.0f, 1.0f));
	auto cubeObj2 = cubeObj;
	cubeObj2.modelMat.translate(glm::vec3(0.0f, 3.0f, -3.0f));
	auto cubeObj3 = cubeObj2;
	cubeObj3.modelMat.translate(glm::vec3(0.0f, 3.0f, -3.0f));
	auto cubeObj4 = cubeObj3;
	cubeObj4.modelMat.translate(glm::vec3(0.0f, 3.0f, -3.0f));
	auto cubeObj5= cubeObj4;
	cubeObj5.modelMat.translate(glm::vec3(0.0f, 3.0f, -3.0f));
	auto cubeObj6= cubeObj5;
	cubeObj6.modelMat.translate(glm::vec3(0.0f, 3.0f, -3.0f));
	
	//objects.emplace_back(std::make_unique<Model<TexMesh>>(std::move(cubeObj)));
	objects.emplace_back(std::make_unique<Model<TexMesh>>(std::move(cubeObj2)));
	objects.emplace_back(std::make_unique<Model<TexMesh>>(std::move(cubeObj3)));
	objects.emplace_back(std::make_unique<Model<TexMesh>>(std::move(cubeObj4)));
	objects.emplace_back(std::make_unique<Model<TexMesh>>(std::move(cubeObj5)));
	objects.emplace_back(std::make_unique<Model<TexMesh>>(std::move(cubeObj6)));
	objects.emplace_back(std::make_unique<Model<TexMesh>>(std::move(floorObj)));

    // Set lights
	
	//openGlCtx.setDirLight(DirLight(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f), *sp2Ptr));
	auto pointLight = openGlCtx.addPointLight(PointLight(glm::vec3(1.0f, 0.5f, 0.5f), *sp2Ptr));
	pointLight->modelMat.translate(glm::vec3(2.0f, 2.0f, 2.0f));
	pointLight = openGlCtx.addPointLight(PointLight(glm::vec3(0.0f, 0.0f, 1.0f), *sp2Ptr));
	pointLight->modelMat.translate(glm::vec3(0.0f, 5.0f, -2.0f));
	auto spotLight = openGlCtx.addSpotLight(SpotLight(glm::vec3(1.0f), *sp2Ptr, glm::vec3(0.0f, -0.8f, 1.0f), 30.0f));
	spotLight->modelMat.translate(glm::vec3(-6.0f, 2.0f, 4.0f));

	return objects;
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

	bool isWireframeMode = false;

	// End of GUI controls settings //
	
    OpenGLCtx openGlCtx;
	std::vector<std::unique_ptr<Object3D>> objects;
	
	try
	{		
		openGlCtx.init();
		objects = prepareScene(openGlCtx);
	}
	catch (std::exception& e)
	{
		printf(e.what());
		return 1;
	}
		
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
    	
    	processKbInput(window, openGlCtx);
    	setCameraRotation(openGlCtx);

    	auto timeCurrentFrame = glfwGetTime();
    	timeDelta = static_cast<float>(timeCurrentFrame - timeLastFrame);
    	timeLastFrame = timeCurrentFrame;

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

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);

    	openGlCtx.setWireframeMode(isWireframeMode);
        openGlCtx.render(display_w, display_h, objects.begin(), objects.end());
        openGlCtx.renderLights(display_w, display_h);

        cursorDeltaX = 0;
        cursorDeltaY = 0;
    	
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

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
#include "Motorbike.h"


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

bool init = true;
bool freeCamera = true;

const float mouseSensitivity = 0.1f;
const float scrollSensitivity = 6.0f;
float cursorLastX = 0;
float cursorLastY = 0;
float cursorDeltaX = 0;
float cursorDeltaY = 0;
float cameraSpeedMult = 1.0f;
float followCameraRotationDeg = 0.0f;

float timeDelta = 0.0;
double timeLastFrame = 0.0;

struct InstancedSceneGraphNodes
{
	std::vector<SceneGraphNode*> nodes;
	std::vector<InstancedElementDraw*> drawImpls;
};

struct SceneData
{
	std::vector<InstancedSceneGraphNodes> instancedSceneGraphNodes;
	Camera followCamera;
	FPSCamera fpsCamera;
	SceneGraphNode* dirLightNode;
	MotorbikeNode* motorbikeNode;
};

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

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
		freeCamera ^= 1;
}

void processKbInput(GLFWwindow* window, OpenGLCtx& openGlCtx, SceneData& sceneData)
{
	auto moveIncrement = 2.5f * timeDelta * cameraSpeedMult;
    auto ifPressed = [window](int key)->bool { return glfwGetKey(window, key) == GLFW_PRESS; };

	if (freeCamera)
	{
		if (ifPressed(GLFW_KEY_W))
			sceneData.fpsCamera.moveFB(moveIncrement);
		if (ifPressed(GLFW_KEY_S))
	        sceneData.fpsCamera.moveFB(-moveIncrement);
		if (ifPressed(GLFW_KEY_A))
	        sceneData.fpsCamera.moveLR(moveIncrement);
		if (ifPressed(GLFW_KEY_D))
	        sceneData.fpsCamera.moveLR(-moveIncrement);
		if (ifPressed(GLFW_KEY_SPACE))
	        sceneData.fpsCamera.moveUD(moveIncrement);
		if (ifPressed(GLFW_KEY_LEFT_SHIFT))
	        sceneData.fpsCamera.moveUD(-moveIncrement);
	}
	else
	{
		if (ifPressed(GLFW_KEY_W))
			sceneData.motorbikeNode->accelerate();
		if (ifPressed(GLFW_KEY_S))
	        sceneData.motorbikeNode->applyBreaks();
		if (ifPressed(GLFW_KEY_A))
	        sceneData.motorbikeNode->turnLeft();
		if (ifPressed(GLFW_KEY_D))
	        sceneData.motorbikeNode->turnRight();
	}
	
}

void setupCamera(OpenGLCtx& openGlCtx, SceneData& sceneData)
{
	if (freeCamera)
	{
		openGlCtx.setCamera(&sceneData.fpsCamera);
		float pitchDeg = -cursorDeltaY * mouseSensitivity;
		float yawDeg = cursorDeltaX * mouseSensitivity;
		sceneData.fpsCamera.rotate(pitchDeg, yawDeg);
	}
	else
	{
		followCameraRotationDeg += cursorDeltaX * mouseSensitivity;
		
		openGlCtx.setCamera(&sceneData.followCamera);
		glm::vec3 motorbikePos = sceneData.motorbikeNode->getObject()->modelMat.getTMat()[3];
		sceneData.followCamera.setCenter(motorbikePos + glm::vec3(0.0f, 3.0f, 0.0f));
		
		auto rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(followCameraRotationDeg), glm::vec3(0.0f, 1.0f, 0.0f));
		auto motorbikeDir = sceneData.motorbikeNode->getDirection();
		glm::vec4 directionVec4(motorbikeDir.x, motorbikeDir.y, motorbikeDir.z, 0.0f);
		glm::vec3 cameraDir = rotMat * directionVec4;
    	sceneData.followCamera.setPosition(motorbikePos + (cameraDir * -15.0f + glm::vec3(0.0f, 4.0f, 0.0f)));
	}
}

template <typename T>
std::vector<InstancedElementDraw*> prepareInstancedRendering(Model<T>& model, GLsizei instancesCount)
{
	std::vector<InstancedElementDraw*> drawImpls;
	
	for (auto&& mesh : model.getMeshes())
	{
		std::shared_ptr<InstancedElementDraw> instancedDraw = std::make_shared<InstancedElementDraw>(mesh.getElementsCount(), instancesCount);
        instancedDraw->init(mesh.getOpenGLRender().getShaderProgram(), mesh.getOpenGLRender().getVaoId());
		
		mesh.getOpenGLRender().drawImpl = instancedDraw;
		drawImpls.push_back(instancedDraw.get());
	}

	return drawImpls;
}

void updateInstancedNodes(std::vector<InstancedSceneGraphNodes>& instancedSceneGraphNodes)
{
	for (auto&& instancedNodes : instancedSceneGraphNodes)
	{
		std::vector<glm::mat4> iMats;
		for (auto&& node: instancedNodes.nodes)
		{
			iMats.push_back(node->getObject()->modelMat.getTMat());
		}
		
		for (auto&& drawImpl : instancedNodes.drawImpls)
		{
			drawImpl->setInstancesData(iMats);
		}
	}
}

SceneData prepareScene(OpenGLCtx& openGlCtx, SceneGraphNode* rootNode)
{
	const std::string skyboxTexturesDir = ".\\res\\textures\\skybox";
	
	SceneData sceneData;
	
	// Prepare shaders
	
	Shader basicVert(".\\res\\shaders\\basic.vert", GL_VERTEX_SHADER);
	Shader basicInstVert(".\\res\\shaders\\basic_instanced.vert", GL_VERTEX_SHADER);
	Shader skyboxVert(".\\res\\shaders\\skybox.vert", GL_VERTEX_SHADER);
	Shader reflectiveVert(".\\res\\shaders\\reflective.vert", GL_VERTEX_SHADER);	
	Shader basicFrag(".\\res\\shaders\\basic.frag", GL_FRAGMENT_SHADER);
	Shader lightFrag(".\\res\\shaders\\light.frag", GL_FRAGMENT_SHADER);
	Shader skyboxFrag(".\\res\\shaders\\skybox.frag", GL_FRAGMENT_SHADER);
	Shader reflectiveFrag(".\\res\\shaders\\reflective.frag", GL_FRAGMENT_SHADER);
	Shader refractiveFrag(".\\res\\shaders\\refractive.frag", GL_FRAGMENT_SHADER);
	
	
	basicVert.compileShader();
	basicInstVert.compileShader();
	skyboxVert.compileShader();
	reflectiveVert.compileShader();
	basicFrag.compileShader();
	lightFrag.compileShader();
	skyboxFrag.compileShader();
	reflectiveFrag.compileShader();
	refractiveFrag.compileShader();

	ShaderProgram basicShader;
	basicShader.attachShader(basicVert);
	basicShader.attachShader(basicFrag);
	basicShader.makeProgram();

    ShaderProgram lightShader;
	lightShader.attachShader(basicVert);
	lightShader.attachShader(lightFrag);
	lightShader.makeProgram();

    ShaderProgram basicInstancedShader;
	basicInstancedShader.attachShader(basicInstVert);
	basicInstancedShader.attachShader(basicFrag);
	basicInstancedShader.makeProgram();

	ShaderProgram skyboxShader;
	skyboxShader.attachShader(skyboxVert);
	skyboxShader.attachShader(skyboxFrag);
	skyboxShader.makeProgram();
	skyboxShader.setAttribPosByName("pos_in");

	ShaderProgram reflectiveShader;
	reflectiveShader.attachShader(reflectiveVert);
	reflectiveShader.attachShader(reflectiveFrag);
	reflectiveShader.makeProgram();

	ShaderProgram refractiveShader;
	refractiveShader.attachShader(reflectiveVert);
	refractiveShader.attachShader(refractiveFrag);
	refractiveShader.makeProgram();
	
    auto spPtr = openGlCtx.addShaderProgram(std::move(basicShader));
	auto sp2Ptr = openGlCtx.addShaderProgram(std::move(lightShader));
	auto sp3Ptr = openGlCtx.addShaderProgram(std::move(basicInstancedShader));
	auto sp4Ptr = openGlCtx.addShaderProgram(std::move(reflectiveShader));
	auto sp5Ptr = openGlCtx.addShaderProgram(std::move(refractiveShader));
	
	// Load and setup models

	AssimpModelLoader<TexMesh> modelLoader(".\\res\\models", ".\\res\\textures");
	auto groundObj = modelLoader.loadModel("ground.obj", *spPtr, aiColor4D{0.1f, 0.1f, 0.1f, 1.0f});
	auto houseObj = modelLoader.loadModel("domek.obj", *sp3Ptr);
	auto roofObj = modelLoader.loadModel("dach.obj", *sp3Ptr);

	auto motorbikeNode = rootNode->attachChildren(std::make_unique<MotorbikeNode>(spPtr, sp4Ptr, sp5Ptr, skyboxTexturesDir));
	motorbikeNode->localMat.translate(glm::vec3(0.0f, 2.24782f, 0.0f));
	sceneData.motorbikeNode = dynamic_cast<MotorbikeNode*>(motorbikeNode);
	
	const int housesInRowCount = 25;
	const int housesInColCount = 25;
	const float housesSpacing = 5.0f;
	
    std::vector<glm::mat4> iMats;
	for (int i = 0; i < housesInRowCount; ++i)
	{
		for (int j = 0; j < housesInColCount; ++j)
		{
			TMat mat;
			mat.translate(glm::vec3(i * housesSpacing, 0.0f, j * housesSpacing));
			iMats.push_back(mat.getTMat());
		}
	}
	
	auto houseDrawImpls = prepareInstancedRendering(houseObj, housesInColCount * housesInRowCount);
	auto roofDrawImpls = prepareInstancedRendering(roofObj, housesInColCount * housesInRowCount);
	
	rootNode->attachChildren(NODE_FROM_MODEL(groundObj));
	auto* housesRootNode = rootNode->attachChildren();
	housesRootNode->localMat.translate(glm::vec3(-990.0f, 0.0f, -990.0f));
	housesRootNode->attachChildren(NODE_FROM_MODEL(houseObj))
				  ->attachChildren(NODE_FROM_MODEL(roofObj));
	housesRootNode->localMat.scale(glm::vec3(8.0f));

	std::vector<SceneGraphNode*> houseNodes;
	std::vector<SceneGraphNode*> roofNodes;
	
	for (auto&& iMat : iMats)
	{
		static bool firstIt = true;

		SceneGraphNode* houseNode;
		SceneGraphNode* roofNode;

		if (firstIt)
		{
			houseNode = housesRootNode->getChildrens()[0].get();
			roofNode = houseNode->getChildrens()[0].get();

			firstIt = false;
		}
		else
		{
			houseNode = housesRootNode->attachChildren();
			roofNode = houseNode->attachChildren();
		}
		
		houseNode->localMat.setTMat(iMat);
		roofNode->localMat.setTMat(TMat().translate(glm::vec3(0.0f, 2.0f, 0.0f)));

		houseNodes.push_back(houseNode);
		roofNodes.push_back(roofNode);
	}

	// Set lights

	auto dirLightNode = rootNode->attachChildren(NODE_FROM_MODEL(DirLight(glm::vec3(1.0f), *sp2Ptr)));
	dirLightNode->localMat.translate(glm::vec3(0.0f, 3.0f, 0.0f));
	openGlCtx.setDirLight(dynamic_cast<DirLight*>(dirLightNode->getObject()));
	
	rootNode->updateModelMats();

	std::vector<InstancedSceneGraphNodes> instancedSceneGraphNodes;
	instancedSceneGraphNodes.push_back({std::move(houseNodes), std::move(houseDrawImpls)});
	instancedSceneGraphNodes.push_back({std::move(roofNodes), std::move(roofDrawImpls)});

	sceneData.instancedSceneGraphNodes = instancedSceneGraphNodes;
	sceneData.dirLightNode = dirLightNode;

	openGlCtx.setSkybox(skyboxTexturesDir, skyboxShader);

	return sceneData;
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
	bool dirLightOn = true;
	bool dirLightObjVisible = false;

	glm::vec3 dirLightColor = glm::vec3(1.0f);
	glm::vec3 dirLightDirection = glm::vec3(1.0f, -1.0f, -1.0f);

	// End of GUI controls settings //
	
    OpenGLCtx openGlCtx;
	std::unique_ptr<SceneGraphNode> rootNode = std::make_unique<SceneGraphNode>();
	SceneData sceneData;
	bool sceneGraphWasDirty = true;
	
	try
	{		
		openGlCtx.init();
		sceneData = prepareScene(openGlCtx, rootNode.get());
	}
	catch (std::exception& e)
	{
		printf(e.what());
		return 1;
	}
		
	glfwSetScrollCallback(window, mouseScrollCallback);
	glfwSetKeyCallback(window, keyboardCallback);
	
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
    	
    	processKbInput(window, openGlCtx, sceneData);

    	auto timeCurrentFrame = glfwGetTime();
    	timeDelta = static_cast<float>(timeCurrentFrame - timeLastFrame);
    	timeLastFrame = timeCurrentFrame;

    	// Check if any of the nodes were dirty in the last frame.
    	// If yes, update instance matrices for instanced objects.
    	// NOTE: This causes the update (of instance matrices) being late by 1 frame after the actual change of the translation matrices.
		if (sceneGraphWasDirty)
		{
			updateInstancedNodes(sceneData.instancedSceneGraphNodes);
		}

		sceneData.dirLightNode->updateModelMats();
    	
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            ImGui::Begin("OpenGL App");
        	ImGui::Checkbox("Wireframe mode", &isWireframeMode);
        	
        	ImGui::Text("Directional light");
        	ImGui::Checkbox("Enabled", &dirLightOn); ImGui::SameLine(); ImGui::Checkbox("Object visible", &dirLightObjVisible);
        	ImGui::ColorEdit3("Color", glm::value_ptr(dirLightColor));
        	ImGui::SliderFloat3("Direction", glm::value_ptr(dirLightDirection), -1.0f, 1.0f);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
    	
        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);

    	openGlCtx.setWireframeMode(isWireframeMode);    	
    	auto dirLight = openGlCtx.getDirLight();
    	dirLight->changeState(dirLightOn, dirLightObjVisible);
    	dirLight->setColor(dirLightColor);
    	dirLight->setDirection(dirLightDirection);

    	sceneData.motorbikeNode->animate();

    	setupCamera(openGlCtx, sceneData);
    	
        openGlCtx.render(display_w, display_h, rootNode.get(), sceneGraphWasDirty);
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

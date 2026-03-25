#include "GameEngine.h"
#include <iostream>

const int winWidth = 640;
const int winHeight = 480;

namespace GE 
{

	GameEngine::GameEngine()
	{
	}

	GameEngine::~GameEngine()
	{
	}

	bool GameEngine::init() {
		// Initialise SDL video subsystem
		if (SDL_Init(SDL_INIT_VIDEO) < 0) 
		{
			std::cerr << "Unable to initialise SDL!  SDL error: " << SDL_GetError() << std::endl;

			return false;
		}

		// Set the OpenGL version for the program
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		// Set the type of profile which is core meaning modern OpenGL
		// means no legacy features for backwards compatibility
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

		// Create the window and frame features
		// In this case window has a fixed size and prepares window for OpenGL to render into
		window = SDL_CreateWindow("SDL OpenGL", 50, 50, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		// Check window was created
		if (window == nullptr) 
		{
			// Display an error if window wasn't created
			// Could log the errors to a file or create more robust error handling
			// e.g. give specific errors a number
			std::cerr << "Unable to create window! SDL error: " << SDL_GetError() << std::endl;

			// Return failure so caller can work out what to do
			return false;
		}

		// Creates the OpenGL context and links it to the window object
		// Context represents OpenGL for program such as objects and drawing
		glContext = SDL_GL_CreateContext(window);

		// Check context object was created
		if (glContext == nullptr) 
		{
			// Display error and return failure
			std::cerr << "SDL could not create GL context! SDL error: " << SDL_GetError() << std::endl;
			return false;
		}

		// Initialise GLEW.  GLEW solves a problem with OpenGL on windows
		// as default API supported by Windows is 1.1 (from 1997)
		// GLEW binds latest extensions that can be used
		GLenum status = glewInit();

		// Check GLEW initialised ok
		if (status != GLEW_OK) 
		{
			std::cerr << "Error initialising GLEW! Error: " << glewGetErrorString(status) << std::endl;

			return false;
		}

		// Try to turn on VSync.  Maybe this could be a parameter to the init method
		// rather than hard coded
		if (SDL_GL_SetSwapInterval(1) != 0) 
		{
			std::cerr << "Warning: unable to set VSync! Error: " << SDL_GetError() << std::endl;

			return false;
		}

		// Look down Z axis into the screen
		dist = glm::vec3(0.0f, 0.0f, -100.0f);

		//Create the camera object
		cam = new Camera(glm::vec3(0.0f, 0.0f, 50.0f), // Position
			glm::vec3(0.0f, 0.0f, -1.0f), // Look at 
			glm::vec3(0.0f, 1.0f, 0.0f), // Up direction
			66.0f, (float) winWidth / winHeight, 0.1f, 1000.0f); // Fov, aspect ratio based on window dimesions, near and far clip planes

		// Create FPSCameraController object
		fpsController = new FPSCameraController(cam);

		//Create the Model object
		m = new Model();

		// Load vertices from monkey.obj file
		bool result = m->loadFromFile(".\\space_frigate_6.obj");

		// Check it was loaded okay
		if (!result) 
		{
			std::cerr << "Failed to load model" << std::endl;
		}

		tex = new Texture(".\\space_frigate_6_color.jpg");

		// Create a ModelRenderer passing the Model object just loaded
		mr = new ModelRenderer();

		// Initialise the ModelRenderer, reduce the scale
		mr->init();
		
		mr->setPos(0.5f, 0.5f, -20.0f);
		mr->setTexture(tex);

		skybox = new SkyboxRenderer
		(
			"front.jpg", "back.jpg", 
			"right.jpg", "left.jpg", 
			"top.jpg", "bottom.jpg"
		);

		terrain = new Terrain();

		terrainTexture = new Texture(".\\terrain-texture.png");

		mrTerrain = new ModelRenderer();
		mrTerrain->init();
		mrTerrain->setTexture(terrainTexture);

		// Load a texture for the billboard 
		bbTex = new Texture(".\\tree.png");
		
		// By default billboard will be at 0,0,0 
		// in front of camera
		bb = new Billboard(bbTex);
		
		// Scale billboard as only 1.0x1.0 
		bb->setScaleX (10.0f); 
		bb->setScaleY(10.0f);
		
		// Set it in front of the model 
		bb->setZ (-10.0f);
		
		// Create the billboard renderer 
		bbr = new BillboardRenderer();
		
		// Initialse the renderer 
		bbr->init();

		// Got this far then must have been successful setting up SDL and OpenGL
		return true;
	}

	// A game would call this function to check if the user closed the window
	// resulting in a SQL_QUIT message
	bool GameEngine::keep_running()
	{
		// Need to do this keep the event queue up to date
		SDL_PumpEvents();

		SDL_Event evt;

		// Check for quit event
		if (SDL_PeepEvents(&evt, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT)) 
		{
			// If user quit program, then return false meaning
			// don't keep running
			return false;
		}

		// Got this far means no quit event so keep running
		return true;
	}

	// Update method which updates the game logic
	// Does nothing for this example
	void GameEngine::update() 
	{
		mr->setRotation(0.0f, mr->getRotY() + 2.5f, 0.0f);

		// Rotate camera at location
		glm::mat4 cam_rot = glm::mat4(1.0f);
		
		// Create a rotation matrix of 0.625 of a degree (per frame) on the Y axis 
		cam_rot = glm::rotate(cam_rot, glm:: radians (0.625f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		// Create a vec4 in order to multiply against rotation matrix
		// Note the last value is 0.0f as want to rotate direction not translate
		glm::vec4 temp = glm::vec4(dist, 0.0f);
		
		// Rotate the dist
		temp = temp * cam_rot;
		
		// Create a new dist based on the rotation 
		dist = glm::vec3(temp.x, temp.y, temp.z);
		
		// Update camera 
		fpsController->update();
	}

	// Draw method.  Will modify this to render more complex scenes
	// For now clears the background
	void GameEngine::draw() 
	{
		glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render skybox. Do this first
		skybox->draw(cam);

		// Draw terrain
		mrTerrain->draw(cam, terrain);

		// Render the model using the model renderer
		mr->draw(cam, m);

		bbr->draw(bb, cam);

		SDL_GL_SwapWindow(window);
	}

	// Shutdown method closes OpenGL and SDL down
	// Will need to modify when we create additional objects
	void GameEngine::shutdown() 
	{
		// Get the ModelRenderer object to release it's resources

		mrTerrain->destroy();
		mr->destroy();
		skybox->destroy();

		// Delete owned objects in order: controller -> camera -> renderers/models
		delete terrain;

		delete skybox;

		delete mr;

		delete m;

		// Delete FPS controller before deleting the camera because controller holds cam pointer
		if (fpsController) 
		{
			delete fpsController;
			fpsController = nullptr;
		}

		delete cam;

		// Free billboard objects
		delete bbr;
		delete bb;
		delete bbTex;

		SDL_DestroyWindow(window);
		window = nullptr;

		SDL_Quit();
	}

	// Simple helper method to set the text in window title bar
	void GameEngine::setwindowtitle(const char* new_title) 
	{
		SDL_SetWindowTitle(window, new_title);
	}

	// Simple helper function to display program information
	// Note it is separate from the GameEngine class as it doesn't depend on it
	void display_info_message(const char* msg) 
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Basic Game Engine", msg, nullptr);
	}
}
#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "Camera.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "SkyboxRenderer.h"
#include "FPSCameraController.h"
#include "Billboard.h"
#include "BillboardRenderer.h"

namespace GE 
{
	// Basic Game Engine class
	class GameEngine 
	{
	public:
		GameEngine();			// Constructor

		virtual ~GameEngine();	// Destructor

		bool init();			// Object initialisation
		bool keep_running();	// Indicates if user has closed window and, hence, game
		void update();			// This is where we update game logic
		void draw();			// This is where a frame is rendered
		void shutdown();		// This is called when the game is ending
								// to release objects created during the game

		// Extra methods
		void setwindowtitle(const char*);

	private:
		// Private data
		// SDL window object to represent the frame
		SDL_Window* window;

		// SDL Gl context to represent OpenGL in the program
		SDL_GLContext glContext;

		// Camera
		Camera* cam;

		// FPS Camera Controller
		FPSCameraController* fpsController;

		// Add code for distance/direction to look from camera
		glm::vec3 dist;

		// Terrain
		Terrain* terrain;
		Texture* terrainTexture;
		ModelRenderer* mrTerrain;

		// Model objects to renderer
		Model* m;
		Texture* tex;
		ModelRenderer* mr;

		Texture* bbTex;
		Billboard* bb;
		BillboardRenderer* bbr;

		SkyboxRenderer* skybox;
	};

	// Helper function
	void display_info_message(const char*);
}
#pragma once
#include <SDL.h>
#include <iostream>
#include "Camera.h"

namespace GE
{

	class FPSCameraController
	{
	public:
		// 7. Define constructor
		FPSCameraController(Camera* cam)
		{
			// Camera object controlled by the controller 
			theCam = cam;

			// Time used to determine move step
			oldTicks = SDL_GetTicks();

			// Get the window dimensions in order to determine centre 
			SDL_Window* window = SDL_GL_GetCurrentWindow();

			int w, h;

			SDL_GetWindowSize(window, &w, &h);

			// Set mouse to middle of window from which to work out 
			// a deflection
			SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), w / 2, h / 2);
		}

		// Destructor.  Does nothing
		virtual ~FPSCameraController() {}

		// 8. Update camera position based on keypressed.  Probably needs
		// a handle input method as this method is quite large
		void update() {
			// Movement constants
			const float mouseSens = 0.1f; // adjust accordingly should be part of the engine variables 
			const float moveDist = 5.0f;

			// Get time now
			Uint32 newTicks = SDL_GetTicks();

			// Calculate elapsed time for determining move amount later
			Uint32 diffTicks = newTicks - oldTicks;

			// Get window width and height in order to work out midpoint
			// used to determine deflection
			SDL_Window* window = SDL_GL_GetCurrentWindow();

			int w, h;

			SDL_GetWindowSize(window, &w, &h);

			// Get current mouse x and y
			int mouse_x, mouse_y;
			SDL_GetMouseState(&mouse_x, &mouse_y);

			// Calculate the deflection - note inversion on y as this is flipped 
			int diffx = mouse_x - (w / 2);
			int diffy = (h / 2) - mouse_y;


			// Update the yaw and pitch based on the mouse deflection 
			theCam->setYaw(theCam->getYaw() + diffx * mouseSens);
			theCam->setPitch(theCam->getPitch() + diffy * mouseSens);

			// Read keyboard directly
			const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

			// Move in direction based on time elapsed since last update
			if (keyboardState[SDL_SCANCODE_UP])
			{

				theCam->setPos(theCam->getPos() + theCam->getLookDir() * (moveDist * diffTicks / 1000.0f));
			}

			if (keyboardState[SDL_SCANCODE_DOWN])
			{

				theCam->setPos(theCam->getPos() - theCam->getLookDir() * (moveDist * diffTicks / 1000.0f));
			}

			if (keyboardState[SDL_SCANCODE_LEFT])
			{

				// Take away the cross product of the facing and up vector from the camera position - strafe left
				theCam->setPos(theCam->getPos() - glm::normalize(glm::cross(theCam->getLookDir(), theCam->getUpDir())) * (moveDist * diffTicks / 1000.0f));
			}

			if (keyboardState[SDL_SCANCODE_RIGHT])
			{
				// Take away the cross product of the facing and up vector from the camera position - strafe right
				theCam->setPos(theCam->getPos() + glm::normalize(glm::cross(theCam->getLookDir(), theCam->getUpDir())) * (moveDist * diffTicks / 1000.0f));
			}

			if (keyboardState[SDL_SCANCODE_W])
			{

				theCam->setPos(theCam->getPos() + theCam->getLookDir() * (moveDist * diffTicks / 1000.0f));
			}

			if (keyboardState[SDL_SCANCODE_S])
			{

				theCam->setPos(theCam->getPos() - theCam->getLookDir() * (moveDist * diffTicks / 1000.0f));
			}

			if (keyboardState[SDL_SCANCODE_A])
			{

				// Take away the cross product of the facing and up vector from the camera position - strafe left
				theCam->setPos(theCam->getPos() - glm::normalize(glm::cross(theCam->getLookDir(), theCam->getUpDir())) * (moveDist * diffTicks / 1000.0f));
			}

			if (keyboardState[SDL_SCANCODE_D])
			{
				// Take away the cross product of the facing and up vector from the camera position - strafe right
				theCam->setPos(theCam->getPos() + glm::normalize(glm::cross(theCam->getLookDir(), theCam->getUpDir())) * (moveDist * diffTicks / 1000.0f));
			}

			// Set mouse to middle of window
			SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), w / 2, h / 2);

			oldTicks = newTicks;
		}

	private:
		// 9. Member variables
		Camera* theCam;
		Uint32 oldTicks;
	};


}
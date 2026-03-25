#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GE 
{
	class Camera 
	{
	public:
		Camera(glm::vec3 cam_pos, glm::vec3 cam_forwardDir, glm::vec3 cam_upDir, float fov, float ar, float near, float far) 
		{
			// Initialise the camera variables to the values passed on the parameters
			pos = cam_pos;

			// Forward direction not location
			forwardDir = cam_forwardDir;
			up = cam_upDir;

			// Initialise the look direction to default forward direction
			lookDir = forwardDir;

			// Initialise pitch and yaw
			pitch = 0.0f;
			yaw = 0.0f;

			fovy = fov;
			aspectR = ar;
			nearClip = near;
			farClip = far;

			updateCamMatrices();
		}

		~Camera() 
		{

		}

		// Accessor methods
		// Return the position of the camera in terms of x, y, z
		float getPosX() 
		{
			return pos.x;
		}

		float getPosY() 
		{
			return pos.y;
		}

		float getPosZ() 
		{
			return pos.z;
		}

		glm::vec3 getPos() 
		{
			return pos;
		}

		float getPitch() 
		{
			return pitch;
		}

		float getYaw() 
		{
			return yaw;
		}

		// Target or what location
		const glm::vec3 getForwardDir()
		{
			return forwardDir;
		}

		const glm::vec3 getLookDir()
		{
			return lookDir;
		}

		// Up direction
		glm::vec3 getUpDir()
		{
			return up;
		}

		// Return the camera's view matrix.  Used by draw
		// method to send view matrix to vertex shader
		glm::mat4 getViewMatrix()
		{
			return viewMat;
		}

		// Return camera's projection matrix.  Same reason
		// as above
		glm::mat4 getProjectionMatrix()
		{
			return projectionMat;
		}

		// Mutator methods
		// Set position
		void setPosX(float newX) 
		{
			pos = glm::vec3(newX, pos.y, pos.z);

			updateCamMatrices();
		}

		void setPosY(float newY) 
		{
			pos = glm::vec3(pos.x, newY, pos.z);

			updateCamMatrices();
		}

		void setPosZ(float newZ) 
		{
			pos = glm::vec3(pos.x, pos.y, newZ);

			updateCamMatrices();
		}

		// Set position for all axes in one method
		void setPos(glm::vec3 newPos) 
		{
			pos = newPos;

			updateCamMatrices();
		}

		void setPitch(float newPitch) 
		{
			pitch = newPitch;
			
			// Clamp the pitch values so we can't point down at our feet or look straight up
			if (pitch > 70.0f) pitch = 70.0f;
			if (pitch < -70.0f) pitch = -70.0f;

			updateCamMatrices();
		}

		void setYaw(float newYaw) 
		{
			yaw = newYaw;
			
			updateCamMatrices();
		}

		// Set new target
		void setForwardDir(glm::vec3 newDir)
		{
			forwardDir = newDir;

			updateCamMatrices();
		}

		// Set the up direction
		void setUpDir(glm::vec3 newUp) 
		{
			up = newUp;

			updateCamMatrices();
		}

		// Set the fov
		void setFov(float newFov) 
		{
			fovy = newFov;

			updateCamMatrices();
		}

		// Set the aspect ratio.
		// You might use this method if the program's window
		// can be resized meaning the width and height change
		void setAspectRatio(float newAR)
		{
			aspectR = newAR;

			updateCamMatrices();
		}

		// Set clipping planes
		void setNearClip(float newNearClip) 
		{
			nearClip = newNearClip;

			updateCamMatrices();
		}

		void setFarClip(float newFarClip) 
		{
			farClip = newFarClip;

			updateCamMatrices();
		}

	private:
		// Update the camera matrices done in response to
		// a member variable variable being updated
		void updateCamMatrices() 
		{
			glm::mat4 rollpitchyawmat = glm::mat4(1.0f);

			// Calculate rotation matrix. Note, -yaw because rotations in OpenGL are counter clockwise
			rollpitchyawmat = glm::rotate(rollpitchyawmat, glm::radians(-yaw), glm::vec3(0.0f, 1.0f, 0.0f));
			rollpitchyawmat = glm::rotate(rollpitchyawmat, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));

			viewMat = glm::lookAt(pos, pos + lookDir, up);
			projectionMat = glm::perspective(glm::radians(fovy), aspectR, nearClip, farClip);
		}

		// Member variables
		// Camera view variables
		glm::vec3 pos;
		glm::vec3 forwardDir;
		glm::vec3 up;

		// Stores the direction camera is facing
		glm::vec3 lookDir;

		// Camera orientation
		float pitch, yaw;
		
		// Camera projection variables
		float fovy;
		float aspectR;
		float nearClip;
		float farClip;

		// View and projection matrices
		glm::mat4 viewMat;
		glm::mat4 projectionMat;
	};
}

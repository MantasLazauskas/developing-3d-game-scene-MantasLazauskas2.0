#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Terrain.h"

namespace GE
{
	class ModelRenderer
	{
	public:
		ModelRenderer();

		virtual ~ModelRenderer();

		// init method to create the shaders and vertex buffer object 
		void init();

		// update method to update the state of the object
		// For example, in other types of object may have some animation
		// which needs updating every frame
		void update();


		// draw method to render the triangle
		void draw(Camera* cam, Model* model);

		// 13. draw method for terrains
		// What C++ feature is being used here as
		// the draw name has been used twice and
		// typically cannot use the same name twice
		void draw(Camera* cam, Terrain* terrain);

		// Release method to release the objects such as vertex buffer object 
		void destroy();

		// Accessors
		// Position
		float getPosX()
		{
			return pos_x;
		}

		float getPosY()
		{
			return pos_y;
		}
		float getPosZ()
		{
			return pos_z;
		}

		// Rotation
		float getRotX()
		{
			return rot_x;
		}

		float getRotY()
		{
			return rot_y;
		}

		float getRotZ()
		{
			return rot_z;
		}

		// Scale
		float getScaleX() {
			return scale_x;
		}

		float getScaleY()
		{
			return scale_y;
		}

		float getScaleZ()
		{
			return scale_z;
		}

		// Mutator methods
		void setPos(float x, float y, float z)
		{
			pos_x = x;
			pos_y = y;
			pos_z = z;
		}

		void setRotation(float rx, float ry, float rz)
		{
			rot_x = rx;
			rot_y = ry;
			rot_z = rz;
		}

		void setScale(float sx, float sy, float sz)
		{
			scale_x = sx;
			scale_y = sy;
			scale_z = sz;
		}

		// Texture methods
		void setTexture(Texture* _tex)
		{
			tex = _tex;
		}

	private:
		// Member fields
		// This member stores the program object that contains the shaders 
		GLuint programId;

		// This member stores the attribute to select into the pipeline
		// to link the triangle vertices to the pipeline
		GLint vertexLocation;

		//Link to vColour attribute which recieves a colour
		//and passes to fColour for fragment shader
		GLint vertexUVLocation;

		// Location, rotation and scale variables
		float pos_x, pos_y, pos_z;
		float rot_x, rot_y, rot_z;
		float scale_x, scale_y, scale_z;

		// GLSL uniform variables for the transformation, view and projection matrices
		GLuint transformUniformId;
		GLuint viewUniformId;
		GLuint projectionUniformId;
		GLuint samplerId;

		// Model object to renderer
		Model* model;

		// Texture to apply to model
		Texture* tex;
	};
}
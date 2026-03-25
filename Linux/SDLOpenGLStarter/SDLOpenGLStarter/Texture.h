#pragma once
#include <GL/glew.h> 
#include <SDL.h>
#include <SDL_image.h> 
#include <string>

namespace GE
{
	class Texture
	{
	public:
		// Constructor
		Texture(std::string filename)
		{
			// Initialise fields.
			width = 0;
			height = 0;
			
			textureName = 0;
			
			// Load the texture into OpenGL 
			loadTexture(filename);
		}
		~Texture() 
		{

		}
		
		// Accessor methods. 
		int getWidth() 
		{ 
			return width;
		}
		
		int getHeight() 
		{
			return height;
		}
		
		Uint32 getFormat()
		{
			return format;
		}
		
		GLuint getTextureName() 
		{
			return textureName;
		}
	
	private:
		// Helper function to load the texture 
		void loadTexture(std::string filename);
	
	private:
		// Dimensions
		int width;
		int height;
		
		// Format
		Uint32 format;
		// OpenGL name for texture object 
		GLuint textureName;
	};
}
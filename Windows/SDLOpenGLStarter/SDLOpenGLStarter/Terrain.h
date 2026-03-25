#pragma once
#include <GL/glew.h> 
#include <SDL.h>
#include <SDL_opengl.h>
namespace GE
{
	class Terrain
	{
	public:
		Terrain();

		// This class has accessors as the 
		// terrain is hard coded in terms of 
		// scale and heightmap file
		// You should modify the class to make
		// easy to configure at runtime
		
		// getVertices works in the same way as the 
		// Model getVertices
		GLuint getVertices() 
		{ 
			return vbo; 
		}

		// getIndices gets the index buffer object 
		// as need to bind to pipeline in order to 
		// render using index buffer
		GLuint getIndices() 
		{
			return ibo;
		}
		
		// Number of indices in the index buffer object 
		GLuint getIndexCount() 
		{ 
			return indexCount; 
		}

	private:
		GLuint vbo, ibo, indexCount;
	};
}
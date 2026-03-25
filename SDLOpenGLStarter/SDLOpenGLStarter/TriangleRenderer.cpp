#include "TriangleRenderer.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace GE 
{
	// Represents a vertex in the engine 
	struct Vertex
	{
		// Location
		float x, y, z;
		float r, g, b, a;

		// Constructors
		// Sets vertex to passed values 
		Vertex(float _x, float _y, float _z, float _r, float _g, float _b, float _a)
		{
			// Location
			x = _x;
			y = _y;
			z = _z;

			// Colour
			r = _r; 
			g = _g; 
			b = _b; 
			a = _a;
		}

		// Sets vertex to origin
		Vertex()
		{
			x = y = z = 0.0f;
			r = g = b = a = 0.0f;
		}
	};

	// Define the triangles's vertices
	Vertex vertexData[] =
	{
		//      xyz Coords         rgba 
		Vertex(-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f)
	};

	// Constructor. Does nothing
	TriangleRenderer::TriangleRenderer() 
	{
		// Initialise location, rotation and scale to default values
		pos_x = pos_y = pos_z = 0.0f;
		rot_x = rot_y = rot_z = 0.0f;
		scale_x = scale_y = scale_z = 1.0f;
	}

	// Destructor. Does nothing
	TriangleRenderer::~TriangleRenderer()
	{

	}

	// This is a helper function that allows us to see
	// shader compiler error messages should our shaders not compile okay 
	void displayShaderCompilerError(GLuint shaderId)
	{
		// First, get the length of the error message string
		GLint MsgLen = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &MsgLen);

		// Only display the message is more than 1 character was returned 
		if (MsgLen > 1)
		{
			// Create a character buffer to store the error message characters 
			GLchar* Msg = new GLchar[MsgLen + 1];

			// Get the error message characters from the OpenGL internal log #
			// into the Msg buffer
			glGetShaderInfoLog(shaderId, MsgLen, NULL, Msg);

			// Display the error message so we can see what the problem 
			// was with the shader
			std::cerr << "Error compiling shader" << Msg << std::endl;

			// Release the memory allocated to the string
			delete[] Msg;
		}
	}


	// Creates and compiles the shaders, creates the project and links it, 
	// and creates the vertex buffer object. Could be argued that method 
	// is not too cohesive
	void TriangleRenderer::init()
	{
		// Create the vertex shader first.
		// Order doesn't matter but shaders must be created and compiled before 
		// attaching to program
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// The shader code as described in lecture
		const GLchar* V_ShaderCode[] =
		{
		"#version 410\n"
		"in vec3 vertexPos3D;\n"
		"in vec4 vColour;\n"
		"out vec4 fColour;\n"
		"uniform mat4 transformMat;\n"
		"uniform mat4 viewMat;\n"
		"uniform mat4 projMat;\n"
		"void main() {\n"
		"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
		"v = projMat * viewMat * transformMatr * v;\n" 
		"gl_Position = v;\n"
		"fColour = vColour;\n"
		"}\n"
		};

		// Copy the source to OpenGL ready for compilation 
		glShaderSource(vertexShader, 1, V_ShaderCode, NULL);

		// Compile the code 
		glCompileShader(vertexShader);

		// Check for compiler errors 
		// Presume shader didn't compile 
		GLint isShaderCompiledOK = GL_FALSE;

		// Get the compile status from OpenGL
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isShaderCompiledOK);

		// Has the shader failed to compile? 
		if (isShaderCompiledOK != GL_TRUE)
		{
			// Yes, so display an error message
			std::cerr << "Unable to compile vertex shader" << std::endl;
			displayShaderCompilerError(vertexShader);
			return;
		}


		// Do the same for the fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Fragment shader source code
		const GLchar* F_ShaderCode[] =
		{
			"#version 410\n"
			"in vec4b fColour;\n"
			"out vec4 fragmentColour;\n"
			"void main()\n"
			"{\n"
			"fragmentColour = fColour;\n"
			"}\n"
		};

		// Transfer the shader code
		glShaderSource(fragmentShader, 1, F_ShaderCode, NULL);

		// Compile it
		glCompileShader(fragmentShader);

		// Check for errors. Code is same as above of getting status 
		// and displaying error message, if necessary
		isShaderCompiledOK = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiledOK);

		if (isShaderCompiledOK != GL_TRUE)
		{
			std::cerr << "Unable to compile fragment shader" << std::endl;

			displayShaderCompilerError(fragmentShader);

			return;
		}

		// Create the program object 
		programId = glCreateProgram();

		// Attach shaders to the program object 
		glAttachShader(programId, vertexShader);
		glAttachShader(programId, fragmentShader);

		// Now link the program to create an executable program we 
		// and use to render the object
		// Program executable will exist in graphics memory
		glLinkProgram(programId);
		
		// Check for linking errors
		GLint isProgramLinked = GL_FALSE;
		glGetProgramiv(programId, GL_LINK_STATUS, &isProgramLinked); 
		if (isProgramLinked != GL_TRUE) 
		{
			std::cerr << "Failed to link program" << std::endl;
		}
		
		// Now get a link to the vertexPos2D so we can link the attribute 
		// to our vertices when rendering
		vertexPos3DLocation = glGetAttribLocation(programId, "vertexPos3D");
		
		// Check for errors
		if (vertexPos3DLocation == -1)
		{
			std::cerr << "Problem getting vertex3DPos" << std::endl;
		}

		// Now get a link to the vColour attribute in the vertex shader
		// so we can indicate to the pipeline where the colour data is to be
		// inserted
		vertexColourLocation = glGetAttribLocation(programId, "vColour");

		// Check for errors
		if (vertexColourLocation == -1) 
		{ 
			std::cerr << "Problem getting vColour" << std::endl; 
		}	

		// Get uniform id in shader so C++ program can send data to it 
		viewUniformId = glGetUniformLocation (programId, "viewMat");
		projectionUniformId = glGetUniformLocation(programId, "projMat"); 
		// Transformation matrix uniform
		transformUniformId = glGetUniformLocation(programId, "transformMat");
		
		// Create the vertex buffer object
		glGenBuffers(1, &vboTriangle);
		glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);
		
		// Transfer vertices to graphics memory
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
		
		// Tidy up after setting up buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void TriangleRenderer::update() 
	{

	}

	// Draw renders the triangles from the buffer object 
	void TriangleRenderer::draw(Camera* cam)
	{

		// Calculate the transformation matrix for the object. Start with the identity matrix 
		glm::mat4 transformationMat = glm::mat4(1.0f);
		
		transformationMat = glm::translate(transformationMat, glm::vec3(pos_x, pos_y, pos_z)); 
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_x), glm::vec3(1.0f, 0.0f, 0.0f));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_y), glm::vec3(0.0f, 1.0f, 0.0f)); 
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_z), glm::vec3(0.0f, 0.0f, 1.0f)); 
		transformationMat = glm::scale(transformationMat, glm::vec3(scale_x, scale_y, scale_z));

		// Get the view and projection matrices
		glm::mat4 viewMat = cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		// Select the program into the rendering context 
		glUseProgram (programId);

		// Set the uniforms in the shader
		glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat)); 
		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));
		
		// Select the vertex buffer object into the context
		glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);
		
		// Enable the attribute to be passed vertices from the vertex buffer object 
		glEnableVertexAttribArray (vertexPos3DLocation);
		
		// Define the structure of a vertex for OpenGL to select values from vertex buffer
		// and store in vertexPos3DLocation attribute
		glVertexAttribPointer(vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, x));

		// Enable the attribute to be passed colours from the vertex buffer object 
		glEnableVertexAttribArray(vertexColourLocation);

		// Define the structure of a vertex for OpenGL to select values from vertex buffer
		// and store in vColour attribute
		glVertexAttribPointer(vertexColourLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));
		
		// Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertexData) / sizeof(Vertex));
		
		// Unselect the attribute from the context 
		glDisableVertexAttribArray (vertexPos3DLocation);

		// Unselect colour attribute from pipeline 
		glDisableVertexAttribArray(vertexColourLocation);
		
		// Unselect vertex buffer 
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// Unselect the program from the context 
		glUseProgram(0);
	}

	void TriangleRenderer::destroy()
	{
		glDeleteProgram(programId);

		glDeleteBuffers(1, &vboTriangle);
	}
}
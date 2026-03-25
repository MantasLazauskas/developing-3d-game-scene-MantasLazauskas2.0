#version 410

// Inputs to vertex shader
in vec3 vertexPos3D;
in vec2 vUV;

// Output
out vec2 uv;

// MVP transformations
uniform mat4 transformMat;
uniform mat4 viewMat;
uniform mat4 projMat;

// Shader main code
void main() {
	// Convert input vertex to 4D 
	vec4 v = vec4(vertexPos3D.xyz, 1);

	// Transform
	v = projMat * viewMat * transformMat * v;

	// Pass transformed vertex to next stage
	gl_Position = v;

	// As well as uv.  Don't transform uv
	uv = vUV;
}
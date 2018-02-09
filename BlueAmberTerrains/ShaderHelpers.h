#pragma once


namespace BlueAmberTerrainsApp
{
	class ShaderHelpers
	{
	public:
		// Build the shaders.
		// Parameters: shaderFiles - list of shaders to build (key: shader type; value: shader file name or relative path).
		// Returns: program handle (a positive integer) on success, zero otherwise.
		// Throws: std::bad_alloc
		static GLuint build(const std::map<GLenum, std::string>& shaderFiles);

	private:
		// Compile the shader.
		// Parameters: shader - OpenGL shader object;
		//             fileName - shader file name (for logging).
		static bool compileShader(GLuint shader, const std::string& fileName);
	};
}

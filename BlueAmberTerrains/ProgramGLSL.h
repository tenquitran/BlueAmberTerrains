#pragma once


namespace BlueAmberTerrainsApp
{
	// Collection of shaders.
	// Key: shader type; value: name of the shader file.
	typedef std::map<GLenum, std::string> ShaderCollection;


	// GLSL program; encapsulates shaders.
	class ProgramGLSL
	{
	public:
		// Parameters: shaders - description of the shaders to load and build.
		// Throws: Exception, std::bad_alloc
		explicit ProgramGLSL(const ShaderCollection& shaders);

		virtual ~ProgramGLSL();

		GLuint getProgram() const;

	private:
		ProgramGLSL(const ProgramGLSL&);
		ProgramGLSL& operator=(const ProgramGLSL&);

	private:
		// GLSL program.
		GLuint m_program;

		// Data on shader files to load and build.
		const ShaderCollection m_shaderFiles;
	};
}

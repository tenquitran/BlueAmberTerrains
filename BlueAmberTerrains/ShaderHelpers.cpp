#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace BlueAmberTerrainsApp;

//////////////////////////////////////////////////////////////////////////


GLuint ShaderHelpers::build(const std::map<GLenum, std::string>& shaderFiles)
{
	if (shaderFiles.empty())
	{
		std::cerr << "No shader files to build\n";
		return 0;
	}

	GLuint programId = {};

	// Create GLSL program and shader objects.

	programId = glCreateProgram();
	if (0 == programId)
	{
		std::cerr << "glCreateProgram() failed\n";
		return 0;
	}

	// Process the list of shaders.

	std::vector<GLuint> shaderIds = {};

	for (const auto& itrShader : shaderFiles)
	{
		// Read the shader's source.

		const std::string fileName = itrShader.second;

		std::ifstream shaderFile(fileName);
		if (!shaderFile.good())
		{
			std::cerr << "Failed to load shader file \"" << fileName << "\"\n";
			return 0;
		}

		std::istreambuf_iterator<char> itrStream(shaderFile);

		std::string src;
		src.assign(itrStream, std::istreambuf_iterator<char>());

		shaderFile.close();

		// Create shader.

		GLuint shader = glCreateShader(itrShader.first);
		if (0 == shader)
		{
			std::cerr << "glCreateShader() failed\n";
			return 0;
		}

		shaderIds.push_back(shader);

		// Compile the shader and add it to the GLSL program.

		const GLchar *pBuff = static_cast<const GLchar*>(src.c_str());

		glShaderSource(shader, 1, (const GLchar**)&pBuff, nullptr);

		if (!compileShader(shader, fileName))
		{
			std::cerr << "Failed to compile shader \"" << fileName << "\"\n";
			return 0;
		}

		glAttachShader(programId, shader);
	}

	// Link the program and check the results.

	glLinkProgram(programId);

	GLint resLink = GL_FALSE;
	glGetProgramiv(programId, GL_LINK_STATUS, &resLink);

	if (GL_TRUE != resLink)
	{
		GLint logLen = {};
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLen);

		if (logLen > 0)
		{
			std::unique_ptr<char[]> spLog = std::make_unique<char[]>(logLen);

			GLsizei written;
			glGetProgramInfoLog(programId, logLen, &written, spLog.get());

			std::cerr << "Failed to link the program: " << spLog.get() << '\n';

#if _DEBUG
			CAtlString msg;
			msg.Format(L"Failed to link the program:\n%S\n", spLog.get());
			::OutputDebugStringW(msg);
#endif
		}

		return 0;
	}

	// Validate the program and check the results.

	glValidateProgram(programId);

	GLint resValidation = GL_FALSE;
	glGetProgramiv(programId, GL_VALIDATE_STATUS, &resValidation);

	if (GL_TRUE != resValidation)
	{
		GLint logLen = {};
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLen);

		if (logLen > 0)
		{
			std::unique_ptr<char[]> spLog = std::make_unique<char[]>(logLen);

			GLsizei written;
			glGetProgramInfoLog(programId, logLen, &written, spLog.get());

			std::cerr << "Failed to validate the program: " << spLog.get() << '\n';

#if _DEBUG
			CAtlString msg;
			msg.Format(L"Failed to validate the program:\n%S\n", spLog.get());
			::OutputDebugStringW(msg);
#endif
		}

		return 0;
	}

	// Clean up the shaders data.
	for (const auto& itr : shaderIds)
	{
		glDetachShader(programId, itr);
		glDeleteShader(itr);
	}

	return programId;
}

bool ShaderHelpers::compileShader(GLuint shader, const std::string& fileName)
{
	glCompileShader(shader);

	GLint res = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);

	if (GL_TRUE != res)
	{
		GLint logLen = {};
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

		if (logLen > 0)
		{
			std::unique_ptr<char[]> spLog = std::make_unique<char[]>(logLen);

			GLsizei written;
			glGetShaderInfoLog(shader, logLen, &written, spLog.get());

			std::cerr << "Failed to compile shader \"" << fileName << "\":\n" << spLog.get() << '\n';

#if _DEBUG
			CAtlString msg;
			msg.Format(L"Failed to compile shader %S:\n%S\n", fileName.c_str(), spLog.get());
			::OutputDebugStringW(msg);
#endif
		}

		return false;
	}

	return true;
}

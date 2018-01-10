#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include "Common.hpp"

class Shader {
public:
	// Constructor generates the shader on the fly
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	// Uses the current shader
	void Use();

private:
	GLuint programId_;
};

#endif // !_SHADER_HPP_


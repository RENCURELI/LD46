#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string & filePath)
	: m_FilePath(filePath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filePath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const std::string & name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "WARNING : Uniform '" << name << "' doesn't exist" << std::endl;

	m_UniformLocationCache[name] = location;
	return location;
}


ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath); //input file stream

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos) //string::npos = invalid string position
		{
			if (line.find("Vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("Fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			//Now that shader type is dfined, add all consecutive lines to string stream
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str() , ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); //OpenGL takes raw C++ data types, so get first char of string
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//Would execute error handling around here
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) //usage of OpenGL can be changed to == 0 or !result
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char)); //alloca allows to allocate dynamically on the stack
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "A " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " Shader FAILED to compile !" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id); //Shader not useful
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram(); //createProgram returns unsigned int
	//Create both shaders
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	//Shaders have been linked into program and can be deleted
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

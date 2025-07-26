#include "shader.h"
#include "glsl/glsl.h"

#include <iostream>


std::unordered_map<char, std::pair<GLenum, size_t>> Shader::_types {
	{'b', {GL_BYTE, sizeof(GLbyte)}},
	{'B', {GL_UNSIGNED_BYTE, sizeof(GLubyte)}},
	{'s', {GL_SHORT, sizeof(GLshort)}},
	{'B', {GL_UNSIGNED_SHORT, sizeof(GLushort)}},
	{'i', {GL_INT, sizeof(GLint)}},
	{'I', {GL_UNSIGNED_INT, sizeof(GLuint)}},
	{'f', {GL_FLOAT, sizeof(GLfloat)}},
	{'d', {GL_DOUBLE, sizeof(GLdouble)}}
};

ShaderVAO::ShaderVAO(int id, const std::string &vertexCode, const std::string &fragmentCode,
                     const std::string &vertexFormat) : Shader(id, vertexCode, fragmentCode, vertexFormat) {
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    setupVertices();

}

Shader::Shader(int id, const std::string& vertexCode, const std::string& fragmentCode,
			   const std::string& vertexFormat) : _id(id), _stride(0), _flags(0u) {

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	GLint result;
	GLuint vid = glCreateShader(GL_VERTEX_SHADER);
	GLuint fid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vid, 1, &vShaderCode, 0);
	glShaderSource(fid, 1, &fShaderCode, 0);

	glCompileShader(vid);
	glGetShaderiv(vid, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result) {
		std::cerr << "Error while compiling vertex shader\n";
		GLint blen;
		glGetShaderiv(vid, GL_INFO_LOG_LENGTH, &blen);
		if (blen > 1) {
			char* compiler_log = (char*)malloc(blen);
			glGetInfoLogARB(vid, blen, 0, compiler_log);
			std::cerr << compiler_log << "\n";
			free(compiler_log);
		}
		glDeleteShader(vid);
		glDeleteShader(fid);
	}

	glCompileShader(fid);
	glGetShaderiv(fid, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result) {
		std::cerr << "Error while compiling fragment shader\n";
		GLint blen;
		glGetShaderiv(fid, GL_INFO_LOG_LENGTH, &blen);
		if (blen > 1) {
			char* compiler_log = (char*)malloc(blen);
			glGetInfoLogARB(fid, blen, 0, compiler_log);
			std::cerr << compiler_log << "\n";
			free(compiler_log);
		}
		glDeleteShader(vid);
		glDeleteShader(fid);

	}

	GLuint progId = glCreateProgram();
	glAttachShader(progId, vid);
	glAttachShader(progId, fid);
	glLinkProgram(progId);

	glGetProgramiv(progId, GL_LINK_STATUS, &result);
	if (GL_FALSE == result) {
		std::cerr << "Error while linking program\n";
		GLchar infoLog[1024];
		glGetProgramInfoLog(progId, 1024, NULL, infoLog);
		std::cerr << infoLog << "\n";
		exit(1);

	}
	_programId = progId;

	size_t start = 0;

	for (size_t i = 0; i < vertexFormat.size(); ++i) {
		if (std::isalpha(vertexFormat[i])) {
			auto size = std::stoi(vertexFormat.substr(start, i-start));
			const auto& typeInfo = _types.at(vertexFormat[i]);
			_vertexFormat.emplace_back(VertexInfo{size, typeInfo.first, size * (unsigned long)typeInfo.second});
			start = i+1;
			_stride += size * typeInfo.second;
		}
	}

}


Shader::~Shader() {
	glDeleteProgram(_programId);
}

void Shader::init() {
	glUseProgram(_programId);
}

void Shader::use() {
	glUseProgram(_programId);
}


void ShaderVAO::use() {
    Shader::use();
    //glBindVertexArray(_vao);
}
void Shader::setupVertices() {
	GLuint i{0};
	unsigned long ptr{0};
	for (const auto& vertex : _vertexFormat) {
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, vertex.size, GL_FLOAT, GL_FALSE, _stride, (void*)ptr);
		ptr += vertex.byteSize;
		i++;
	}
}


void Shader:: setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(_programId, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(_programId, name.c_str()), value);
}

ShaderStore::ShaderStore() {
	_shaderBuilders[1] = [&] () { return std::make_shared<Shader>(1, color_vs, color_fs, "3f4f"); };
    _shaderBuilders[2] = [&] () { return std::make_shared<Shader>(2, color_normal_vs, color_normal_fs, "3f4f3f"); };
    _shaderBuilders[3] = [&] () { return std::make_shared<Shader>(3, tex_vs, tex_fs, "3f2f1i"); };
	_shaderBuilders[4] = [&] () { return std::make_shared<Shader>(3, tex_vs, tex_fs_pal, "3f2f1i"); };
	_shaderBuilders[5] = [&] () { return std::make_shared<Shader>(5, tex_vs, agi_pic_fs, "3f2f1i"); };

    _shaderBuilders[70] = [&] () { return std::make_shared<ShaderVAO>(70, skeletal_vs, skeletal_fs, "3f2f3f"); };

}

Shader * ShaderStore::getShader(int id) {
	auto it = _shaders.find(id);
	if (it != _shaders.end()) {
		return it->second.get();
	}
	auto shader = _shaderBuilders.at(id)();
	_shaders[id] = shader;
	return shader.get();
}


void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	auto pippo = glGetUniformLocation(_programId, name.c_str());
	glUniformMatrix4fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	auto pippo = glGetUniformLocation(_programId, name.c_str());
	glUniformMatrix3fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(_programId, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(_programId, name.c_str()), 1, &value[0]);
}



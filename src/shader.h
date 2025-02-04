#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "unordered_map"

class Node;

struct VertexInfo {
	int size;
	GLenum type;
	unsigned long byteSize;
};



class Shader {
public:
	Shader(int id, const std::string& vertexCode, const std::string& fragmentCode, const std::string& vertexFormat);
	~Shader();
	virtual void use();
	void init();
	void setupVertices();

	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	[[nodiscard]] GLuint getProgId() const;
	virtual void init(Node*) { }

	virtual void draw() {}

	int getId() const;
protected:
	int _id;
	GLuint _programId;
	std::vector<VertexInfo> _vertexFormat;
	GLsizei _stride;
	static std::unordered_map<char, std::pair<GLenum, size_t>> _types;
	unsigned _flags;
};

inline int Shader::getId() const {
	return _id;
}

inline GLuint Shader::getProgId() const {
	return _programId;
}

class ShaderStore {
public:
	ShaderStore();
	Shader* getShader(int);
    bool hasShader(int) const;
private:
	std::unordered_map<int, std::shared_ptr<Shader>> _shaders;
	std::unordered_map<int, std::function<std::shared_ptr<Shader>()>> _shaderBuilders;

};

inline bool ShaderStore::hasShader(int id) const {
    return _shaders.count(id) > 0;
}

class ShaderVAO : public Shader {
public:
    ShaderVAO(int id, const std::string& vertexCode, const std::string& fragmentCode, const std::string& vertexFormat);
    void use() override;

private:
    GLuint  _vao;
};
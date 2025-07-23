#pragma once

#include <list>
#include <iostream>
#include "shader.h"
#include "camera.h"
#include "light.h"

class IBatch {
public:
	IBatch(int shaderType, int camId);

    virtual ~IBatch() = default;

	virtual void setupUniforms(Shader* s);

	int getShaderType() const;

	//void addLight(std::shared_ptr<Light> light);

	virtual void configure() = 0;

	virtual void draw() = 0;

	int getTextureWidth() const;

	int getTextureHeight() const;

	// transorm (x, y, w, h) coords into tex coords
	glm::vec4 pixelToTextureCoords(glm::ivec4);

	// book a primitive. A primitive is a sequence of n vertices, where
	// n = PRIMITIVE::nVertices
	int getPrimitiveId();

	virtual void release(int id);

	virtual void showPrimitive(int id, bool) {}

	void start();

    int getId() const;

	int getCameraId() const;

	static void resetId() { _gId=0;}
protected:
    // this is the batch unique id
    int _id;

	int _camId;
	int _nPrimitive;					// next element to be allocated
	std::list<int> _deallocated;		// list of element id to recycle
	int _shaderType;
	Camera* _cam;
	//std::vector<std::shared_ptr<Light>> _lights;
    static int _gId;
	int _texWidth;
	int _texHeight;
	float _invWidth;
	float _invHeight;
};

inline int IBatch::getTextureWidth() const {
	return _texWidth;
}

inline int IBatch::getTextureHeight() const {
	return _texHeight;
}

inline int IBatch::getId() const {
    return _id;
}

inline int IBatch::getCameraId() const {
	return _camId;
}
inline int IBatch::getShaderType() const {
	return _shaderType;
}

template<typename VERTEX>
class Batch : public IBatch {
public:

	Batch(int verticesPerElement, int indicesPerElement, GLenum primitive, int n, int shaderType, int cam) : IBatch(shaderType, cam),
		_indicesPerElement(indicesPerElement), _verticesPerPrimitive(verticesPerElement), _size(n),
		_data(n * verticesPerElement), _vao(0), _vbo(0), _ebo(0), _prim(primitive) {
		_indices.resize(n * indicesPerElement, 0);
		_vertexSize = sizeof(VERTEX);
	}

	// return start address of i-th primitive
	VERTEX* getAddress(int i) {
		return &_data[i * _verticesPerPrimitive];
	}

	virtual ~Batch() {
		glDeleteVertexArrays(1, &_vao);
		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_ebo);
	}

	//virtual void initVertices() = 0;
	//virtual std::vector<unsigned> initIndices() = 0;

	void configure() override {
		//initVertices();
		//auto totalVertices = _size * _verticesPerPrimitive;
		//_data.resize(totalVertices);

		// create vertex array object
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		// create buffer
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, _vertexSize * _data.size(), &_data[0], GL_DYNAMIC_DRAW);

		VERTEX::setupVertices();

		initIndices();

		glGenBuffers(1, &_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), &_indices[0], GL_STATIC_DRAW);
	}

	void showPrimitive(int id, bool value) override {
		int offset = _verticesPerPrimitive * id;
		for (int i = 0; i < _verticesPerPrimitive; ++i) {
			_data[offset+i].show(value);
		}
	}




	void draw() override {

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, _vertexSize * _nPrimitive * _verticesPerPrimitive, &_data[0]);

		//initDraw(s);
		glBindVertexArray(_vao);
		glDrawElements(_prim, _indicesPerElement * _nPrimitive, GL_UNSIGNED_INT, (GLvoid*)0);
		glBindVertexArray(0);
	}

	void cleanUp() {
		_nPrimitive = 0;
		_deallocated.clear();
		_data = std::vector<VERTEX>(_size);
	}

	void release(int index) override {
		IBatch::release(index);
		//_deallocated.push_back(index);
		int offset = index * _verticesPerPrimitive;
		memset(&_data[offset], 0, _vertexSize * _verticesPerPrimitive);
	}

	virtual void initIndices() {
		_indices.resize(_size * _indicesPerElement);
		int base = 0;
		int j = 0;
		for (int i = 0; i < _size; ++i) {
			for (int k = 0 ; k < _indicesPerElement; ++k) {
				_indices[j++] = base + _indexPattern[k];
			}
			base += _verticesPerPrimitive;
		}
	}


protected:
	// index of next primitive
	//int _bytesPerVertex;
	int _indicesPerElement;
	GLenum _prim;
	unsigned _verticesPerPrimitive;
	GLuint _vao;
	GLuint _vbo;
	GLuint _ebo;
	//GLint _blockSize;
	size_t _vertexSize;
	int _size;	// total # of vertices: must be primitives * vertex per primitive

	std::vector<VERTEX> _data;
	std::vector<unsigned> _indices;
	std::vector<unsigned> _indexPattern;

};



#pragma once

#include <list>
#include <iostream>
#include "shader.h"
#include "camera.h"
#include "light.h"

class IBatch {
public:
	IBatch(int size, int camId);

    virtual ~IBatch() = default;

	virtual void setupUniforms(Shader* s);

	int getShaderType() const;

	void addLight(std::shared_ptr<Light> light);
	virtual void configure() = 0;
	virtual void draw() = 0;

	// book a primitive. A primitive is a sequence of n vertices, where
	// n = PRIMITIVE::nVertices
	int getPrimitiveId();

	virtual void release(int id);
	void start();

    int getId() const;

	int getCameraId() const;

	static void resetId() { _gId=0;}
protected:
    // this is the batch unique id
    int _id;
	int _size;
	int _camId;
	int _nPrimitive;					// next element to be allocated
	std::list<int> _deallocated;		// list of element id to recycle
	int _shaderType;
	Camera* _cam;
	std::vector<std::shared_ptr<Light>> _lights;
    static int _gId;
};


inline int IBatch::getId() const {
    return _id;
}

inline int IBatch::getCameraId() const {
	return _camId;
}
inline int IBatch::getShaderType() const {
	return _shaderType;
}

template<typename PRIMITIVE>
class Batch : public IBatch {
public:

	using Vertex = typename PRIMITIVE::Vertex;

	Batch(int size, int cam) : IBatch(size, cam), _vao(0), _vbo(0), _ebo(0) {
		_vertexSize = sizeof(Vertex);
		_verticesPerPrimitive = PRIMITIVE::_nVertices;
		_indicesPerElement = PRIMITIVE::_nIndices;
		_data.resize(_size * PRIMITIVE::_nVertices);
		_shaderType = PRIMITIVE::_shaderType;
		_prim = PRIMITIVE::_glPrim;
	}

	virtual ~Batch() {
		glDeleteVertexArrays(1, &_vao);
		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_ebo);
	}

	void configure() override {
		auto totalVertices = _size * _verticesPerPrimitive;
		_data.resize(totalVertices);

		// create vertex array object
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		// create buffer
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, _vertexSize * _data.size(), &_data[0], GL_DYNAMIC_DRAW);

		// can we do this in a smart way ?? innerConfigure();
		Vertex::setupVertices();
		// this depends on the particular batch and should go in a virtual method
		std::vector<unsigned> indices;

		for (size_t i = 0; i < _size; ++i) {
		    int offset = i * _verticesPerPrimitive;
		    for (size_t j = 0; j < _indicesPerElement; j++) {
		        indices.push_back(offset + PRIMITIVE::_indices[j]);
		    }
		}

		glGenBuffers(1, &_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
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
		_data = std::vector<typename PRIMITIVE::Vertex>(_size);
	}

	void release(int index) override {
		IBatch::release(index);
		//_deallocated.push_back(index);
		int offset = index * _verticesPerPrimitive;
		memset(&_data[offset], 0, _vertexSize * _verticesPerPrimitive);
	}

	//virtual void updatePrimitive(int index, const PRIMITIVE& triangle, const glm::mat4&) = 0;


	typename PRIMITIVE::Vertex* getPrimitive(int id) {
		return &_data[id * _verticesPerPrimitive];
	}

	//virtual void initDraw(Shader* s) = 0;
	//virtual void innerConfigure() = 0;
	//virtual void computeOffsets(GLuint) = 0;

	virtual int getPalette(const std::string&) { return 0; }


protected:
	// index of next primitive
	//int _bytesPerVertex;
	int _indicesPerElement;
	GLenum _prim;
	unsigned _verticesPerPrimitive;
	GLuint _vao;
	GLuint _vbo;
	GLuint _ebo;
	GLint _blockSize;
	size_t _vertexSize;


	std::vector<Vertex> _data;


};



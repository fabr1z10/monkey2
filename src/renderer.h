#pragma once

#include "batch.h"
#include "node.h"
#include "game.h"
#include "room.h"


class IRenderer {
public:
    IRenderer() : _node(nullptr), _started(false) {}
	void setNode(Node*);
	virtual void update() {
    }
	virtual void start() = 0;
    virtual void updateGeometry () = 0;

    virtual void draw(Shader*) {}
protected:
	Node* _node;
    bool _started;
    bool _dirty;
};


inline void IRenderer::setNode(Node* node) {
	_node= node;
}

template<typename MODEL>
class Renderer : public IRenderer {
public:
	using Primitive = typename MODEL::Primitive;
	using Vertex = typename MODEL::Primitive::Vertex;

	Renderer(MODEL* model, int batchId) : _model(model), _batchId(batchId) {
		_nPrimitives = _model->getPrimitiveCount();
	}



	// at initialization, renderer will request the required number of primitives from the batch
	void start() {
		_batch = dynamic_cast<Batch<Primitive>*>(Game::instance().getRoom()->getBatch(_batchId));

		for (size_t i = 0; i < _nPrimitives; ++i) {
			_primIds.push_back(_batch->getPrimitiveId());
		}
		_vertices = new typename MODEL::Primitive::Vertex*[_nPrimitives];
		int j = 0;
		for (const auto& i : _primIds) {
			_vertices[j++] = _batch->getPrimitive(i);
		}
		_started = true;
        // 1st call to updategeometry
        updateGeometry();
	}



    void updateGeometry() override {
        auto worldTransform = _node->getWorldMatrix();
        for (size_t i = 0; i < _model->getPrimitiveCount(); ++i) {
            _model->get(i).transform(_vertices[i], worldTransform);
        }
    }
protected:
	int _batchId;
	Batch<Primitive>* _batch;
	int _nPrimitives;
	std::vector<int> _primIds;
	Vertex** _vertices;
	MODEL* _model;

};


// a renderer will render one or more primitives
// so it's responsible for converting each primitive to the
// corresponding vertex data in the batch



#pragma once

#include "batch.h"
#include "node.h"
#include "game.h"
#include "room.h"


class IRenderer {
public:
	void setNode(Node*);
	virtual void update() = 0;
	virtual void start() = 0;
protected:
	Node* _node;
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
		_vertices = new typename MODEL::PRIMITIVE::Vertex*[_nPrimitives];
		for (const auto& i : _primIds) {
			_vertices[i] = _batch->getPrimitive(_primIds[i]);
		}

	}

	void update() {
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



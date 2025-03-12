#pragma once

#include "batch.h"
#include "node.h"
#include "game.h"
#include "room.h"


class IRenderer {
public:
    IRenderer() : _node(nullptr), _started(false), _multiplyColor(glm::vec4(1.f)) {}

	virtual ~IRenderer() = default;

	void setNode(Node*);
	virtual void update() {
    }
	virtual void start() = 0;

	virtual void dispose() {}

    virtual void updateGeometry () = 0;

    virtual void draw(Shader*) {}

    virtual void setAnimation(const std::string&) {}

    virtual std::string getAnimation() const;

    void setMultiplyColor(glm::vec4);
protected:
	Node* _node;
    bool _started;
    bool _dirty;
    glm::vec4 _multiplyColor;

};

inline std::string IRenderer::getAnimation() const {
    return "";
}

inline void IRenderer::setMultiplyColor(glm::vec4 color) {
    _multiplyColor = color;
}

inline void IRenderer::setNode(Node* node) {
	_node= node;
}

template<typename MODEL>
class Renderer : public IRenderer {
public:
	using Primitive = typename MODEL::Primitive;
	using Vertex = typename MODEL::Primitive::Vertex;

	Renderer(MODEL* model, int batchId) : _model(model), _batchId(batchId), _disposed(false) {
		_nPrimitives = _model->getPrimitiveCount();
	}

	~Renderer() {
		if (!_disposed)
			for (auto i : _primIds) _batch->release(i);

	}

	void dispose() override {
		if (!_disposed) {
			for (auto i : _primIds) {
				_batch->release(i);
			}
		}
		_disposed = true;
	}
	// at initialization, renderer will request the required number of primitives from the batch
    void start() override {
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
        if (!_started) {
            return;
        }
        auto worldTransform = _node->getWorldMatrix();
        for (size_t i = 0; i < _model->getPrimitiveCount(); ++i) {
            _model->get(i).transform(_vertices[i], worldTransform, _multiplyColor);
        }
    }
protected:
	int _batchId;
	Batch<Primitive>* _batch;
	int _nPrimitives;
	std::vector<int> _primIds;
	Vertex** _vertices;
	MODEL* _model;
	bool _disposed;

};


// a renderer will render one or more primitives
// so it's responsible for converting each primitive to the
// corresponding vertex data in the batch



#pragma once

#include "batch.h"
#include "node.h"
#include "game.h"
#include "room.h"



class IRenderer {
public:
    IRenderer() : _node(nullptr), _started(false) {}

	virtual ~IRenderer() = default;

	void setNode(Node*);
	virtual void update() {
    }
	virtual void start() = 0;

	virtual void dispose() {}

    virtual void updateGeometry () = 0;

	virtual bool updateFrame() { return false;  }

    virtual void draw(Shader*) {}

    virtual void setAnimation(const std::string&) {}

    virtual std::string getAnimation() const;

    //void setMultiplyColor(glm::vec4);
protected:
	Node* _node;
    bool _started;
    bool _dirty;
    //glm::vec4 _multiplyColor;

};

inline std::string IRenderer::getAnimation() const {
    return "";
}

//inline void IRenderer::setMultiplyColor(glm::vec4 color) {
//    _multiplyColor = color;
//}

inline void IRenderer::setNode(Node* node) {
	_node= node;
}

template <typename T>
class Model;
// a renderer just does (primitives, state) -> vertices
// just requires to know: primitive and vertex
// so it really doesn't care about model right?
template<typename PRIMITIVE>
class Renderer : public IRenderer {
public:
	//using Primitive = typename MODEL::Primitive;
	using Vertex = typename PRIMITIVE::Vertex;

	Renderer() = default;

	Renderer(Model<PRIMITIVE>* model, int batchId) : _model(model), _batchId(batchId), _disposed(false), _vertices(nullptr)  {
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
		if (_started) return;

		_batch = dynamic_cast<Batch<PRIMITIVE>*>(Game::instance().getRoom()->getBatch(_batchId));

		for (size_t i = 0; i < _nPrimitives; ++i) {
			_primIds.push_back(_batch->getPrimitiveId());
		}
		_vertices = new typename PRIMITIVE::Vertex*[_nPrimitives];
		int j = 0;
		for (const auto& i : _primIds) {
			_vertices[j++] = _batch->getPrimitive(i);
		}
		_started = true;
        // 1st call to updategeometry
        updateGeometry();
	}



//    void updateGeometry() override {
//        if (!_started) {
//            return;
//        }
//        auto worldTransform = _node->getWorldMatrix();
//        for (size_t i = 0; i < _model->getPrimitiveCount(); ++i) {
//            _model->get(i).transform(_vertices[i], worldTransform, _multiplyColor);
//        }
//    }
protected:
	int _batchId;
	Batch<PRIMITIVE>* _batch;
	int _nPrimitives;
	std::vector<int> _primIds;
	Vertex** _vertices;
	Model<PRIMITIVE>* _model;
	bool _disposed;

};

template <typename T>
class PaletteRenderer : public Renderer<T> {
public:
	PaletteRenderer(Model<T>* model, int batchId, int palette) : Renderer<T>(model, batchId), _palette(palette) {}

	void setPalette(int id) {
		_palette = id;
	}

	void updateGeometry() override {
		if (!this->_started) {
			return;
		}
		auto worldTransform = this->_node->getWorldMatrix();
		for (size_t i = 0; i < !this->_model->getPrimitiveCount(); ++i) {
			this->_model->get(i).transform(this->_vertices[i], worldTransform, _palette);
		}
	}
private:
	int _palette;
};


// a renderer will render one or more primitives
// so it's responsible for converting each primitive to the
// corresponding vertex data in the batch



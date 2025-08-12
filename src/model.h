#pragma once

#include <memory>
#include <vector>
#include "primitives/prim.h"
#include "error.h"
#include "renderer.h"
#include <string>


class IRenderer;


class IModel {
public:
	IModel();
    virtual ~IModel() = default;
	//virtual std::shared_ptr<IRenderer> getRenderer(int batchId) = 0;
    void update() ;

	virtual void draw() = 0;
	virtual void setNode(Node* node) {
		_node = node;
	}
	//virtual int getPrimitiveCount() const = 0;
	virtual std::string getAnimation();
	virtual void setAnimation(const std::string&) {}
	virtual bool hasAnimation(const std::string&) const { return false; }

	virtual void setVisible(bool) = 0;
	virtual void setPalette(int) {}

	void setUpdate(bool);

	int getShaderId() const {return _shaderId;}
protected:
	virtual void updateImpl() = 0;
	int _shaderId;
	Node* _node;
	bool _update;
};

inline void IModel::setUpdate(bool value) {
	_update = value;
}

template<typename VERTEX>
class Model : public IModel {
public:

    Model(int batchId) : IModel(), _vertices(nullptr) {
		// get the batch
		_batch = dynamic_cast<Batch<VERTEX>*>(Game::instance().getRoom()->getBatch(batchId));
		_shaderId = _batch->getShaderType();
		if (_batch == nullptr) {
			GLIB_FAIL("[BATCH] Batch does not exist or doesn't match with vertex definition.");
		}
	}
	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;
	Model(const Model&&) = delete;
	Model& operator=(const Model&&) = delete;
	void allocate(int primitives) {
		_vertices = new VERTEX*[primitives];
		for (size_t i = 0; i < primitives; ++i) {
			int np = _batch->getPrimitiveId();
			_primitives.push_back(np);
			_vertices[i] = _batch->getAddress(np);
		}
	}

	virtual ~Model() {
		//std::cout << "Called dtor\n";
		for (const auto& i : _primitives) _batch->release(i);
		if (_vertices != nullptr) {
			//std::cout << "Deleting _vertices:\n";
			//for (int i = 0; i < _primitives.size(); ++i) {
			//	std::cout << "  [" << i << "] = " << static_cast<void*>(_vertices[i]) << "\n";
			//}
			delete[] _vertices;
			_vertices = nullptr;
		}
	}
//    Model(int size) : IModel() {
//        for (size_t i = 0; i < size; ++i) {
//            _prims.push_back(P());
//        }
//    }
//
//    Model(const std::vector<float>& data) {
//        M_Assert(data.size() % P::_floatsPerPrimitive == 0, "LineModel requires a vector whose size is multiple of 6.");
//
//        for (size_t i = 0; i < data.size(); i += P::_floatsPerPrimitive) {
//            _prims.push_back(P(&data[i]));
//        }
//    }
//
//    const P& get(int id) {
//        return _prims[id];
//    }
//
//
//
//    int getPrimitiveCount() const override {
//        return _prims.size();
//    }
	void setVisible(bool value) override {
		for (const auto& p : _primitives) {
			_batch->showPrimitive(p, value);
		}
	}

protected:
    VERTEX** _vertices;
	Batch<VERTEX>* _batch;
	std::vector<int> _primitives;


};


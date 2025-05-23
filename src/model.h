#pragma once

#include <memory>
#include <vector>
#include "primitives/prim.h"
#include "error.h"
#include "renderer.h"


class IRenderer;


class IModel {
public:
	IModel() = default;
    virtual ~IModel() = default;
	virtual std::shared_ptr<IRenderer> getRenderer(int batchId) = 0;
    virtual int getPrimitiveCount() const = 0;
};


template<typename P>
class Model : public IModel {
public:
    using Primitive = P;

    Model() : IModel() {}

    Model(int size) : IModel() {
        for (size_t i = 0; i < size; ++i) {
            _prims.push_back(P());
        }
    }

    Model(const std::vector<float>& data) {
        M_Assert(data.size() % P::_floatsPerPrimitive == 0, "LineModel requires a vector whose size is multiple of 6.");

        for (size_t i = 0; i < data.size(); i += P::_floatsPerPrimitive) {
            _prims.push_back(P(&data[i]));
        }
    }

    const P& get(int id) {
        return _prims[id];
    }

    std::shared_ptr<IRenderer> getRenderer(int batchId) override {
        return std::make_shared<Renderer<Model<P>>>(this, batchId);
    }

    int getPrimitiveCount() const override {
        return _prims.size();
    }

protected:
    std::vector<P> _prims;




};

//
//template<typename PRIMITIVE>
//class Model : public IModel {
//public:
//
//private:
//	std::vector<PRIMITIVE> _primitives;
//};
//

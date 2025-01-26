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
	virtual std::shared_ptr<IRenderer> getRenderer(int batchId) = 0;

};


template<typename P>
class Model : public IModel {
public:
    using Primitive = P;

    Model(const std::vector<float>& data, glm::vec4 color) {
        M_Assert(data.size() % P::_floatsPerPrimitive == 0, "LineModel requires a vector whose size is multiple of 6.");

        for (size_t i = 0; i < data.size(); i += P::_floatsPerPrimitive) {
            _prims.push_back(P(&data[i], color));
        }
    }

    const P& get(int id) {
        return _prims[id];
    }

    std::shared_ptr<IRenderer> getRenderer(int batchId) override {
        return std::make_shared<Renderer<Model<P>>>(this, batchId);
    }

    int getPrimitiveCount() {
        return _prims.size();
    }

private:
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

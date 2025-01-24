#pragma once

#include <memory>
#include <vector>
#include "primitives/prim.h"

class IRenderer;


class IModel {
public:
	IModel() = default;
	virtual std::shared_ptr<IRenderer> getRenderer(int batchId) = 0;

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

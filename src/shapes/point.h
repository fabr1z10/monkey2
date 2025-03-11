#pragma once

#include "../shape.h"
#include <vector>


namespace shapes {

	class Point : public Shape {
	public:
		Point();

		bool raycastY(glm::vec2 origin, int dir) const override;

        std::shared_ptr<IModel> makeModel(ModelType type) override;

		bool isInside(glm::vec2) const override;

	};

}

#pragma once

#include "../shape.h"
#include <vector>


namespace shapes {

	class Point : public Shape {
	public:
		Point();

		RaycastResult raycastX(glm::vec2 origin, float length) const override;

		RaycastResult raycastY(glm::vec2 origin, float length) const override;
        std::shared_ptr<IModel> makeModel(int, ModelType type) override;

		bool isInside(glm::vec2) const override;

	};

}

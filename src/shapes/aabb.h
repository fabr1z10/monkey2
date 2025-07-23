#pragma once

#include "../shape.h"
#include "../vec.h"
#include <vector>


namespace shapes {

	class AABB : public Shape {
	public:
		AABB(float width, float height);

		AABB(float width, float height, Vec2 anchor);

		RaycastResult raycastX(glm::vec2 origin, float length) const override;

		RaycastResult raycastY(glm::vec2 origin, float length) const override;

		std::shared_ptr<IModel> makeModel(int, ModelType type) override;

		bool isInside(glm::vec2) const override;

	private:
		float _width;
		float _height;
		glm::vec2 _bl;
	};
}

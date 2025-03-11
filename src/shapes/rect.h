#pragma once

#include "../shape.h"
#include <vector>


namespace shapes {

	class Rect : public Shape {
	public:
		Rect(float width, float height, glm::vec2 anchor = glm::vec2(0.f));

		bool raycastY(glm::vec2 origin, int dir) const override;

        std::shared_ptr<IModel> makeModel(ModelType type) override;

		bool isInside(glm::vec2) const override;

	private:
		float _width;
		float _height;
		glm::vec2 _bl;
	};
}

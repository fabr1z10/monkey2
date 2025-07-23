#pragma once

#include "../shape.h"
#include <vector>
#include "../vec.h"


namespace shapes {

    class Line : public Shape {
    public:
        Line(Vec2 A, Vec2 B);

		RaycastResult raycastX(glm::vec2 origin, float length) const override;

		RaycastResult raycastY(glm::vec2 origin, float length) const override;

        std::shared_ptr<IModel> makeModel(int, ModelType type) override;

		bool isInside(glm::vec2) const override;

	private:
        float x0, x1;
        float y0, y1;
		glm::vec2 _A;
		glm::vec2 _B;
    };

    class PolyLine : public Shape {
    public:
        PolyLine(const std::vector<float>& data);

		RaycastResult raycastX(glm::vec2 origin, float length) const override;

		RaycastResult raycastY(glm::vec2 origin, float length) const override;

        std::shared_ptr<IModel> makeModel(int, ModelType type) override;

		bool isInside(glm::vec2) const override;

		float getY(float x) const;
	private:

        std::vector<float> _x;
        std::vector<float> _y;
    };
}

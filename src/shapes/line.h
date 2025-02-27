#pragma once

#include "../shape.h"
#include <vector>


namespace shapes {

    class Line : public Shape {
    public:
        Line(glm::vec2 A, glm::vec2 B);

        bool raycastY(glm::vec2 origin, int dir) const override;

		std::shared_ptr<IModel> makeModel(glm::vec4 color) override;

		bool isInside(glm::vec2) override;

	private:
        float x0, x1;
        float y0, y1;
		glm::vec2 _A;
		glm::vec2 _B;
    };

    class PolyLine : public Shape {
    public:
        PolyLine(const std::vector<float>& data);

        bool raycastY(glm::vec2 origin, int dir) const override;

		std::shared_ptr<IModel> makeModel(glm::vec4 color) override;

		bool isInside(glm::vec2) override;

	private:

        std::vector<float> _x;
        std::vector<float> _y;
    };
}

#pragma once

#include "../model.h"
#include <vector>
#include "../primitives/vertex.h"

namespace models {


	class LineModel : public Model<VertexColor> {
	public:
		LineModel(int batch, const std::vector<float> &points, Color color);

		//void setX(int id, float);

		//void setY(int id, float);

		void updateImpl() override;

		void draw() override;

	private:
		int _lineCount;
		// local points - we can change them with get/set
		std::vector<glm::vec2> _points;
		glm::vec4 _color;
	};

	class ColorQuadModel : public Model<VertexColor> {
	public:
		ColorQuadModel(int batch, int width, int height, Color color);
		void updateImpl() override;

		void draw() override;
	private:
		glm::vec4 _color;
		float _width;
		float _height;
	};

}


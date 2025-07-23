#pragma once

#include "../model.h"
#include "../primitives/vertex.h"

namespace models {
	/*
	 * Just a basic quad
	 */
	class Quad : public Model<VertexPalette> {
	public:
		Quad(int batch, int x, int y, int width, int height, int pal = 0);

		void update() override;

		void draw() override;

	private:
		glm::ivec4 _quad;
		int _pal;
	};

}
#pragma once

#include "../indexedpng.h"
#include "../batch.h"
#include "../primitives/vertex.h"

namespace agi {
	class PicBatch : public Batch<VertexPalette> {
	public:
		PicBatch(int camera, const std::string &picName);

		void setupUniforms(Shader *);

	private:
		std::unique_ptr<IndexedPNG> _pic;
		std::unique_ptr<IndexedPNG> _priority;
		GLuint _priorityTex;
	};

}
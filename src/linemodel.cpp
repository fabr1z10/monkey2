#include "linemodel.h"
#include "error.h"
#include "renderer.h"

LineModel::LineModel(const std::vector<float> &data, glm::vec4 color) : IModel() {

	M_Assert(data.size() % 6 == 0, "LineModel requires a vector whose size is multiple of 6.");

	for (size_t i = 0; i < data.size(); i+=6) {
		_lines.push_back(primitives::Line(
				glm::vec3(data[i], data[i+1], data[i+2]),
				glm::vec3(data[i+3], data[i+4], data[i+5]),
				color));
	}


}

const primitives::Line & LineModel::get(int id) {
	return _lines[id];
}

std::shared_ptr<IRenderer> LineModel::getRenderer(int batchId) {

	return std::make_shared<Renderer<LineModel>>(this, batchId);
}
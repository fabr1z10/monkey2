#include "linemodel.h"
#include "../game.h"
#include "../linebatch.h"
//#include "renderer.h"
//


using namespace models;


LineModel::LineModel(int batchId, const std::vector<float> &data, Color color) : Model<VertexColor>(batchId), _color(color.toGlm()) {

	// polyline. At least two points required
	M_Assert(data.size() >= 4, "At least 2 points required in polyline.");
	M_Assert(data.size() % 2 == 0, "In polyline, an even number of floats is required.");

	int nPoints = data.size() / 2;
	int nVertices = 2 * (nPoints - 1);
	// check how many primitives do I need
	_lineCount = nPoints-1;

	// allocate n references to vertexColor
	//_vertices = new VertexColor*[nVertices];

	// initialize all stuff - this I think can be moved to model
	for (size_t i = 0; i < data.size(); i += 2) {
		_points.emplace_back(data[i], data[i+1]);
	}

	allocate(_lineCount);
}

void LineModel::update() {
	draw();
}

void LineModel::draw() {
	auto m = _node->getWorldMatrix();

	std::vector<glm::vec3> wPoints;
	wPoints.reserve(_points.size());
	for (const auto& p : _points) {
		wPoints.emplace_back(m * glm::vec4(p, 0.f, 1.f));
	}
	for (size_t i = 0; i < _lineCount; ++i) {
		_vertices[i]->position = wPoints[i];
		_vertices[i]->color = _color;
		(_vertices[i]+1)->position = wPoints[i+1];
		(_vertices[i]+1)->color = _color;
	}



}



//	M_Assert(data.size() % 6 == 0, "LineModel requires a vector whose size is multiple of 6.");
//
//	for (size_t i = 0; i < data.size(); i+=6) {
//		_lines.push_back(primitives::Line(
//				glm::vec3(data[i], data[i+1], data[i+2]),
//				glm::vec3(data[i+3], data[i+4], data[i+5]),
//				color));
//	}
//
//
//}
//
//const primitives::Line & LineModel::get(int id) {
//	return _lines[id];
//}
//
//std::shared_ptr<IRenderer> LineModel::getRenderer(int batchId) {
//
//	return std::make_shared<Renderer<LineModel>>(this, batchId);
//}
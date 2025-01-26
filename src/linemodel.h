//#include "model.h"
//
//#include <vector>
//
//class LineModel : public IModel {
//public:
//	using Primitive = primitives::Line;
//	// defines lines of the same color
//	LineModel(const std::vector<float>& data, glm::vec4 color);
//	const primitives::Line& get(int id);
//	using PRIMITIVE = primitives::Line;
//	std::shared_ptr<IRenderer> getRenderer(int batchId) override;
//	int getPrimitiveCount() {
//		return _lines.size();
//	}
//private:
//	std::vector<primitives::Line> _lines;
//};
//

#include "model.h"

#include <vector>

class TriangleModel : public IModel {
public:
    using Primitive = primitives::Triangle;
    // defines lines of the same color
    TriangleModel(const std::vector<float>& data, glm::vec4 color);
    const primitives::Triangle& get(int id);
    using PRIMITIVE = primitives::Line;
    std::shared_ptr<IRenderer> getRenderer(int batchId) override;
    int getPrimitiveCount() {
        return _lines.size();
    }
private:
    std::vector<primitives::Line> _lines;
};


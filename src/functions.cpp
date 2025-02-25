#include "functions.h"
#include "assetmanager.h"
#include "node.h"
#include "stb_image.h"
#include "shapes/polygon.h"
#include "util.h"
#include "shapes/polyapprox.h"


Game& game() {
	auto& g = Game::instance();
	return g;
}

glm::vec4 fromHex(const std::string& str) {
    unsigned int r = std::stoul(str.substr(1, 2), nullptr, 16);
    unsigned int g = std::stoul(str.substr(3, 2), nullptr, 16);
    unsigned int b = std::stoul(str.substr(5, 2), nullptr, 16);
    return glm::vec4(r / 255.f, g / 255.f, b / 255.f, 1.f);

}

void loadAsset(const std::string& id, const std::string& file, int camera) {
    auto wd = Game::instance().getWorkingDirectory();
    AssetManager::instance().loadAssetFile(id, wd + "/assets/" + file, camera);
}


std::shared_ptr<IModel> getModel(const std::string& id) {
    return AssetManager::instance().getModel(id);
}

Node* getNode(int id) {
    return Node::getNode(id);
}

std::shared_ptr<Shape> shapeFromImage(const std::string& assetBank, int texId, glm::ivec4 coords, int n) {

	auto filePath = AssetManager::instance().getTexturePath(assetBank, texId);
	//filePath = Game::instance().getWorkingDirectory() + "/assets/" + filePath;
	auto points = PolyApproximation::approximatePoly(filePath, coords, n);

	std::vector<float> data;
	for (auto& p : points) {
		data.push_back(p.x);
		data.push_back(p.y);
	}
	return std::make_shared<shapes::Polygon>(data);

}


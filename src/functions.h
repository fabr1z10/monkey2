#include "game.h"


Game& game();

glm::vec4 fromHex(const std::string&);

void loadAsset(const std::string& id, const std::string& file,int camId);

std::shared_ptr<IModel> getModel(const std::string& id);

std::shared_ptr<Shape> shapeFromImage(const std::string& assetBank, int texId, glm::ivec4 coords, int n);


Node* getNode(int);

#include "game.h"


Game& game();

glm::vec4 fromHex(const std::string&);

void loadAsset(const std::string& id, const std::string& file);

std::shared_ptr<IModel> getModel(const std::string& id);

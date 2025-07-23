#pragma once

#include <string>
#include <GL/glew.h>
#include <memory>
#include <unordered_map>
#include "shader.h"
#include "font.h"
#include <yaml-cpp/yaml.h>
#include "models/tilemodel.h"


class SpriteSheet {
public:
	explicit SpriteSheet(const std::string& spritesheetFile);

	static std::shared_ptr<SpriteSheet> getSheet(const std::string& file);

	static void clearSheets();

	void setupUniforms(Shader*);

	glm::ivec2 getTextureSize() const;

	Font* getFont(const std::string&);

	std::shared_ptr<models::TileMap> makeSprite(int batch, const std::string& id);

private:
	void loadFonts(const YAML::Node&);

	void loadSprites(const YAML::Node&);

	static std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sheetCache;

	std::unordered_map<std::string, std::shared_ptr<Font>> _fonts;

	int _texWidth;
	int _texHeight;
	GLuint _texId;
	GLuint _palId;
	int _defaultTicks;

	YAML::Node _sprites;
};

inline glm::ivec2 SpriteSheet::getTextureSize() const {
	return {_texWidth, _texHeight};
}


inline Font *SpriteSheet::getFont(const std::string & id) {
	return _fonts.at(id).get();
}
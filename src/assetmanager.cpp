#include "assetmanager.h"
#include <yaml-cpp/yaml.h>
#include "game.h"
#include "sprite.h"
#include <iostream>
#include "quadbatch.h"
#include "error.h"
#include "yamlcustom.h"

//std::shared_ptr<IModel> IAssetBank::getModel(const std::string& id) {
//	auto it = _models.find(id);
//	if (it == _models.end()) {
//		GLIB_FAIL("Unknown model: " + id);
//	}
//	return it->second;
//}
//
//std::shared_ptr<Font> IAssetBank::getFont(const std::string& id) {
//	auto it = _fonts.find(id);
//	if (it == _fonts.end()) {
//		GLIB_FAIL("Unknown font: " + id);
//	}
//	return it->second;
//}
//
//void AssetManager::startUp()
//{
//    for (auto& b : _banks) {
//        b.second->startUp();
//    }
//}

void AssetManager::loadAssetFile(const std::string& id, const std::string& file, int cam, const std::vector<int>& tex) {
//    auto assetPath = Game::instance().getWorkingDirectory() + "/assets/";
//    std::cout << " -- Loading asset file: " << file << "\n";
//    auto f = YAML::LoadFile(file);
//	std::shared_ptr<IAssetBank> b;
//	if (tex.empty()) {
//		b = std::make_shared<PalAssetBank>(f, cam);
//	} else {
//		b = std::make_shared<AssetBank>(f, cam, tex);
//	}
//	_banks[id] = b;
}




GLuint AssetManager::getTextureId(const std::string &id)
{
    return 0;
}


std::string AssetManager::getTexturePath(const std::string &assetBank, int id) {
	return "";//_banks.at(assetBank)->getTexturePath(id);
}
//std::pair<IAssetBank*, std::string> AssetManager::getBank(const std::string& id) {
//    auto n = id.find('/');
//    auto bankId = id.substr(0, n);
//    auto modelId = id.substr(n+1);
//    auto it = _banks.find(bankId);
//    M_Assert(it != _banks.end(), (("Unknown asset bank ") + bankId).c_str());
//    return {it->second.get(), modelId};
//}

//std::shared_ptr<IModel> AssetManager::getModel(const std::string& id)
//{
//    auto p = getBank(id);
//    return p.first->getModel(p.second);
//}
//
//std::shared_ptr<Font> AssetManager::getFont(const std::string & id)
//{
//    auto p = getBank(id);
//    return p.first->getFont(p.second);
//}

//PalAssetBank::PalAssetBank(const YAML::Node &f, int camId) : IAssetBank(camId) {
//	auto assetPath = Game::instance().getWorkingDirectory() + "/assets/";
//	auto fonts = f["fonts"];
//	auto sprites = f["sprites"];
//	auto quads = f["quads"];
//	auto tex = f["texture"].as<std::string>();
//	// get the palettes
//
//
//	_batch = std::make_shared<PaletteQuadBatch>(10000, _camId, tex, f["palettes"]);
//	Game::instance().getRoom()->addBatch(_batch);
//
//	for (auto f = fonts.begin(); f != fonts.end(); ++f) {
//		auto fontId = f->first.as<std::string>();
//		std::cout << " -- Loading font: " << fontId << std::endl;
//		auto font = std::make_shared<Font>(f->second, _batch.get(), _texId);
//		_fonts[fontId] = font;
//	}
//
//	for (auto s = sprites.begin(); s != sprites.end(); ++s) {
//		auto spriteId = s->first.as<std::string>();
//		std::cout << " -- loading sprite: " << spriteId << "\n";
//		auto sprite = std::make_shared<Sprite>(s->second, _batch.get(), _texId);
//		_models[spriteId] = sprite;
//	}
//
//
//	for (auto q = quads.begin(); q != quads.end(); q++) {
//		auto quadId = q->first.as<std::string>();
//		auto texCoords = q->second["tex"].as<IVec4>();
//		auto anchor = q->second["anchor"].as<IVec2>(IVec2());
//		auto size = q->second["size"].as<IVec2>(IVec2(texCoords.z, texCoords.w));
//		auto pal = q->second["pal"].as<int>(0);
//		_models[quadId] = std::make_shared<PalQuad>(texCoords, anchor, pal, _batch->getId(), size);
//	}
//
//
//}
//
//
//AssetBank::AssetBank(const YAML::Node & f, int camId, const std::vector<int>& textures) : IAssetBank(camId)
//{
//    auto assetPath = Game::instance().getWorkingDirectory() + "/assets/";
//
//    auto node = f["textures"];
//    auto fonts = f["fonts"];
//    auto sprites = f["sprites"];
//    auto quads = f["quads"];
//
//    for (auto c = node.begin(); c!= node.end(); ++c) {
//        auto id = c->first.as<int>();
//		if (std::find(textures.begin(), textures.end(), id) != textures.end()) {
//			auto filepath = c->second.as<std::string>();
//			//std::cout << id << " - " << filepath << std::endl;
//			loadTexture(id, filepath);
//		}
//		// let's store here the filepath .
//
//
//    }
//
//    for (auto f = fonts.begin(); f != fonts.end(); ++f) {
//        auto texId = f->second["tex"].as<int>();
//        // need to map this to shader texture id
//		if (std::find(textures.begin(), textures.end(), texId) != textures.end()) {
//			auto fontId = f->first.as<std::string>();
//            std::cout << " -- Loading font: " << fontId << std::endl;
//			//auto tex = getTexture(texId);
//			auto font = std::make_shared<Font>(f->second, _batch.get(), _textures.at(texId));
//			_fonts[fontId] = font;
//		}
//    }
//
//    for (auto s = sprites.begin(); s != sprites.end(); ++s) {
//        auto texId = s->second["tex"].as<int>();
//		if (std::find(textures.begin(), textures.end(), texId) != textures.end()) {
//			auto spriteId = s->first.as<std::string>();
//
//			std::cout << " -- loading sprite: " << spriteId << "\n";
//			auto sprite = std::make_shared<Sprite>(s->second, _batch.get(), _textures.at(texId));
//			_models[spriteId] = sprite;
//		}
//    }
//
//    for (auto q = quads.begin(); q != quads.end(); q++) {
//        auto texId = q->second["tex"].as<int>();
//		if (std::find(textures.begin(), textures.end(), texId) != textures.end()) {
//			auto quadId = q->first.as<std::string>();
//
//			_models[quadId] = std::make_shared<Quad>(q->second, _batch.get(), _textures.at(texId));
//		}
//    }
//
//}
//
//void AssetBank::loadTexture(int id, const std::string &path)
//{
//    // if this is 1st texture, atuomatically create the batch!
//    if (_textures.empty()) {
//        _batch = std::make_shared<QuadBatch>(10000, _camId, 16);
//
//    } else {
//        // makesure tex have same size
//    }
//    int shaderTexId = _batch->addTexture(path);
//    _textures[id] = shaderTexId;
//	_texPath[id] = path;
//    startUp();
//}
//
//void AssetBank::startUp()
//{
//    Game::instance().getRoom()->addBatch(_batch);
//}
//
//void PalAssetBank::startUp()
//{
//}

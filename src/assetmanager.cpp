#include "assetmanager.h"
#include <yaml-cpp/yaml.h>
#include "game.h"
#include "sprite.h"
#include <iostream>
#include "quadbatch.h"
#include "error.h"



void AssetManager::startUp()
{
    for (auto& b : _banks) {
        b.second->startUp();
    }
}

void AssetManager::loadAssetFile(const std::string& id, const std::string& file, int cam) {
    auto assetPath = Game::instance().getWorkingDirectory() + "/assets/";
    std::cout << " -- Loading asset file: " << file << "\n";
    auto f = YAML::LoadFile(file);
    auto b = std::make_shared<AssetBank>(f, cam);
    _banks[id] = b;
}




GLuint AssetManager::getTextureId(const std::string &id)
{
    return 0;
}


std::string AssetManager::getTexturePath(const std::string &assetBank, int id) {
	return _banks.at(assetBank)->getTexturePath(id);
}
std::pair<AssetBank*, std::string> AssetManager::getBank(const std::string& id) {
    auto n = id.find('/');
    auto bankId = id.substr(0, n);
    auto modelId = id.substr(n+1);
    auto it = _banks.find(bankId);
    M_Assert(it != _banks.end(), (("Unknown asset bank ") + bankId).c_str());
    return {it->second.get(), modelId};
}

std::shared_ptr<IModel> AssetManager::getModel(const std::string& id)
{
    auto p = getBank(id);
    return p.first->getModel(p.second);
}

std::shared_ptr<Font> AssetManager::getFont(const std::string & id)
{
    auto p = getBank(id);
    return p.first->getFont(p.second);
}

AssetBank::AssetBank(const YAML::Node & f, int camId) : _camId(camId)
{
    auto assetPath = Game::instance().getWorkingDirectory() + "/assets/";

    auto node = f["textures"];
    auto fonts = f["fonts"];
    auto sprites = f["sprites"];
    auto quads = f["quads"];

    for (auto c = node.begin(); c!= node.end(); ++c) {
        auto id = c->first.as<int>();
        auto filepath = c->second.as<std::string>();
        std::cout << id << " - " << filepath << std::endl;
        loadTexture(id, filepath);
		// let's store here the filepath .


    }

    for (auto f = fonts.begin(); f != fonts.end(); ++f) {
        auto fontId = f->first.as<std::string>();
        auto texId = f->second["tex"].as<int>();
        // need to map this to shader texture id

        //auto tex = getTexture(texId);
        auto font = std::make_shared<Font>(f->second, _batch.get(), _textures.at(texId));
        _fonts[fontId] = font;
    }

    for (auto s = sprites.begin(); s != sprites.end(); ++s) {
        auto spriteId = s->first.as<std::string>();
        auto texId = s->second["tex"].as<int>();        
        std::cout << " -- loading sprite: " << spriteId << "\n";
        auto sprite = std::make_shared<Sprite>(s->second, _batch.get(), _textures.at(texId));
        _models[spriteId] = sprite;
    }

    for (auto q = quads.begin(); q != quads.end(); q++) {
        auto quadId = q->first.as<std::string>();
        auto texId = q->second["tex"].as<int>();
        _models[quadId] = std::make_shared<Quad>(q->second, _batch.get(), _textures.at(texId));
    }

}

void AssetBank::loadTexture(int id, const std::string &path)
{
    // if this is 1st texture, atuomatically create the batch!
    if (_textures.empty()) {
        _batch = std::make_shared<QuadBatch>(10000, _camId, 16);

    } else {
        // makesure tex have same size
    }
    int shaderTexId = _batch->addTexture(path);
    _textures[id] = shaderTexId;
	_texPath[id] = path;
}

void AssetBank::startUp()
{
    Game::instance().getRoom()->addBatch(_batch);
}


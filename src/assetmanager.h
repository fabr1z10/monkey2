#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "tex.h"
#include "font.h"
#include "model.h"
#include "quadbatch.h"
#include <yaml-cpp/yaml.h>



class AssetBank {
public:
    AssetBank(const YAML::Node&);
    void loadTexture(int id, const std::string& path);
    //Tex* getTexture(int id);
    std::shared_ptr<IModel> getModel(const std::string& id);
    std::shared_ptr<Font> getFont(const std::string& id);
    int getBatchId() const;
    void startUp();
private:
    std::unordered_map<int, int> _textures;
    std::unordered_map<std::string, std::shared_ptr<Font>> _fonts;

    std::unordered_map<std::string, std::shared_ptr<IModel>> _models;
    std::shared_ptr<QuadBatch> _batch;

};

inline std::shared_ptr<IModel> AssetBank::getModel(const std::string& id) {
    return _models.at(id);
}

inline std::shared_ptr<Font> AssetBank::getFont(const std::string& id) {
    return _fonts.at(id);
}
// inline Tex* AssetBank::getTexture(int id) {
//     return _textures.at(id).get();
// }

class AssetManager {
public:
    void startUp();
    static AssetManager &instance() {
        static AssetManager instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    void loadAssetFile(const std::string& id, const std::string& file);

    GLuint getTextureId(const std::string& id);

    std::shared_ptr<IModel> getModel(const std::string&);

    std::shared_ptr<Font> getFont(const std::string&);

private:
    std::pair<AssetBank*, std::string> getBank(const std::string&);

    AssetManager() {}

    std::unordered_map<std::string, std::shared_ptr<AssetBank>> _banks;

};



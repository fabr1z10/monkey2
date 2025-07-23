#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "tex.h"
#include "font.h"
#include "model.h"
#include "linebatch.h"
#include <yaml-cpp/yaml.h>


//class IAssetBank {
//public:
//	IAssetBank(int camId) : _camId(camId) {}
//	std::shared_ptr<IModel> getModel(const std::string& id);
//	std::shared_ptr<Font> getFont(const std::string& id);
//	virtual void startUp() = 0;
//protected:
//	int _camId;
//	std::unordered_map<std::string, std::shared_ptr<Font>> _fonts;
//
//	std::unordered_map<std::string, std::shared_ptr<IModel>> _models;
//
//};
//
//
//class AssetBank : public IAssetBank {
//public:
//
//    AssetBank(const YAML::Node&, int camId, const std::vector<int>& textures);
//    void loadTexture(int id, const std::string& path);
//    //Tex* getTexture(int id);
//    int getBatchId() const;
//    //void startUp();
//	std::string getTexturePath(int id) const;
//	void startUp() override;
//
//private:
//
//    std::unordered_map<int, int> _textures;
//	std::unordered_map<int, std::string> _texPath;
//    std::shared_ptr<QuadBatch> _batch;
//};
//
//
//class PalAssetBank : public IAssetBank {
//public:
//	PalAssetBank(const YAML::Node& node, int camId);
//	void startUp() override;
//private:
//	int _texId;
//	std::shared_ptr<QuadBatchPalette> _batch;
//
//};
//
//inline std::string AssetBank::getTexturePath(int id) const {
//	return _texPath.at(id);
//}

class AssetManager {
public:
    //void startUp();
    static AssetManager &instance() {
        static AssetManager instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    void loadAssetFile(const std::string& id, const std::string& file,int cam, const std::vector<int>& tex);

	std::string getTexturePath(const std::string& assetBank, int);

    GLuint getTextureId(const std::string& id);

   // std::shared_ptr<IModel> getModel(const std::string&);

    //s/td::shared_ptr<Font> getFont(const std::string&);

private:
    //std::pair<IAssetBank*, std::string> getBank(const std::string&);

    AssetManager() {}

    //std::unordered_map<std::string, std::shared_ptr<IAssetBank>> _banks;

};



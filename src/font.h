#pragma once

#include <unordered_map>
#include <yaml-cpp/yaml.h>
#include "quadbatch.h"

struct CharInfo {
    // texture top left
    float tx, ty;
    // size
    float tw, th;
    // width and height in px
    float w, h;
    float advance;
    float oy;
};


class Font {

public:
    Font(const YAML::Node& node, QuadBatch*, int);
    const CharInfo& getCharInfo(char32_t c);
    float getLineHeight() const;
    int getBatchId() const;
    int getTexId() const;
private:
    std::unordered_map<char32_t, CharInfo> m_info;
    float _lineHeight;
    int _texId;
    int _batchId;
};

inline int Font::getBatchId() const {
    return _batchId;
}
inline int Font::getTexId() const {
    return _texId;
}

inline const CharInfo & Font::getCharInfo(char32_t c) {
    return m_info.at(c);
}

inline float Font::getLineHeight() const {
    return _lineHeight;
}
// #include <GL/glew.h>
// #include "batch/quadbatch.h"
// #include <yaml-cpp/yaml.h>

// class SpriteSheet;




// class Font {
// public:
//     Font(SpriteSheet* sheet, const YAML::Node& node);
//     float getLineHeight() const;
//     //QuadBatch* getBatch();
// private:
//     QuadBatch* _batch;
//     float _lineHeight;
//     std::unordered_map<char32_t, CharInfo> m_info;
//     SpriteSheet* _sheet;
// };

// inline float Font::getLineHeight() const {
//     return _lineHeight;
// }

#pragma once

#include "node.h"
#include "font.h"


// align each row left (default), center, or right w.r.t. box
// this only works if a width is specified

enum class HAlign {
    LEFT = 0,
    CENTER = 1,
    RIGHT = 2
};


class Text : public Node {
public:
    Text(const std::string& font, const std::string& text, Color color,
         HAlign align = HAlign::LEFT, float width = 0.f, Vec2 anchor = Vec2());

    void updateText(const std::string&);

    std::string getText() const;

    Vec2 getSize() const;
private:
    struct TextRow {
        TextRow (int iStart) : indexStart(iStart), indexEnd(-1), length(0.f) {}
        int indexStart;
        int indexEnd;
        float length;
    };
    float _width;
    float _lineHeight;
    Font* _font;
    std::string _palId;
    glm::vec2 _size;
    glm::vec3 _offset;
    int _lines;
    int _paletteIndex;
    HAlign _hAlign;
    glm::vec2 _anchor;
    //std::string _sheetId;
    std::string _text;
    Color _color;


};

inline std::string Text::getText() const {
    return _text;
}

inline Vec2 Text::getSize() const {
    return Vec2(_size.x, _size.y);
}

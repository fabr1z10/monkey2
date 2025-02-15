#include "text.h"
#include "assetmanager.h"
#include "util.h"
#include "model.h"

Text::Text(const std::string& font, const std::string &text) : Node() {

    _font = AssetManager::instance().getFont(font).get();

    _lineHeight = _font->getLineHeight();
    _width = 0.f;

    updateText(text);

}


void Text::updateText(const std::string & text) {
    _text = text;
    auto s32 = getString32(text);

    float crl = 0.f;		// current row length
    float cel = 0.f;
    float cwl = 0.f;		// current world length
    int eol = -1;		// eol index
    int cws = -1;

    std::vector<TextRow> rows;
    rows.emplace_back(TextRow(0));
    int i = 0;
    for (const auto& c : s32) {

        if ( c == '\n') {
            rows.back().indexEnd = i;
            rows.back().length = crl;
            rows.emplace_back(TextRow(i+1));
            eol = 0;
            crl = 0;
            cel = 0;
            cwl = 0;
        } else {

            const auto &charInfo = _font->getCharInfo(c);

            if (c == 0x20) {
                eol = i;
                cel = crl;
                crl += charInfo.advance;
                cws = -1;
                cwl = 0;
            } else {
                if (cws == -1) cws = i;
                crl += charInfo.advance;
                cwl += charInfo.advance;
                if (_width > 0 && crl > _width) {
                    rows.back().indexEnd = eol;
                    rows.back().length = cel;
                    rows.emplace_back(TextRow(cws));
                    crl = cwl;
                    cel = 0;
                }
            }
        }
        i++;
    }
    rows.back().length = crl;


    // now that we have defined the rows we can create the quads

    std::vector<float> modelRaw;

    float y = -_lineHeight;

    for (const auto& row : rows) {
        float x{0.f};
        if (_hAlign == HAlign::CENTER) {
            x = (_width - row.length) * 0.5f;
        } else if (_hAlign == HAlign::RIGHT) {
            x = (_width - row.length);
        }
        int ec = row.indexEnd == -1 ? s32.length() : row.indexEnd;
        int len = ec - row.indexStart;
        if (len > 0) {
            for (int i = row.indexStart; i < ec; ++i) {
                const auto &c = _font->getCharInfo(s32[i]);
                glm::vec3 p(x, y, 0.f);
                //auto kwargs = pybind11::dict("pos"_a=glm::vec3(x,y,0.f),  "size"_a=glm::vec2(c.w,c.h), "normalized"_a=true, "pal"_a=_paletteIndex);
                std::vector<float> charData{c.tx, c.ty, c.tw, c.th, -x, -y, _lineHeight * (c.tw/c.th), _lineHeight, (float)_font->getTexId()};
                modelRaw.insert(modelRaw.end(), charData.begin(), charData.end());

                x += c.advance;
            }
        }
        y -= _lineHeight;
    }





    int rowCount {0};
    float maxRowLength = 0;
    for (const auto& row : rows) {
        maxRowLength = std::max(row.length, maxRowLength);
        int ec = row.indexEnd == -1 ? text.length() : row.indexEnd;
        int len = ec - row.indexStart;
        if (len > 0) {
            std::cout << text.substr(row.indexStart, len) << "\n";
            rowCount++;
        }
    }
    std::cout << "n rows = " << rowCount << "\n";
    auto model = std::make_shared<Model<primitives::Quad>>(modelRaw);

    this->setModel(model, _font->getBatchId());





}

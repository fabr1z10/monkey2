#pragma once

#include <string>
#include <GL/glew.h>
#include "unordered_map"
#include <memory>


class Tex {

public:
    [[nodiscard]] int getWidth() const;

	[[maybe_unused]] int getHeight() const;

	static GLuint getTexture(const std::string& file);

private:
	explicit Tex(const std::string& file);
    GLuint _texId;
    int _width;
    int _height;
    int _nrChannels;
    static std::unordered_map<std::string, std::shared_ptr<Tex>> _textures;
};

inline int Tex::getWidth() const {
    return _width;
}

[[maybe_unused]] inline int Tex::getHeight() const {
    return _height;
}

#pragma once

#include <string>
#include <GL/glew.h>


class IndexedPNG {
public:
	IndexedPNG(const std::string&);

	~IndexedPNG();

	unsigned char getPixel(int x, int y);

	void load();

	GLuint getTexId() const;

	GLuint getPalId() const;

	unsigned long getWidth() const;

	unsigned long getHeight() const;

	size_t size() const { return _size; }

	unsigned char operator[](size_t i) const { return _data[i]; }

	const unsigned char* begin() const { return _data; }

	const unsigned char* end() const { return _data + _size; }

private:
	unsigned long _texWidth;
	unsigned long _texHeight;
	unsigned char* _data;
	unsigned char* _pal;
	GLuint _texId;
	GLuint _palId;
	size_t _size;

};

inline GLuint IndexedPNG::getTexId() const {
	return _texId;
}

inline GLuint IndexedPNG::getPalId() const {
	return _palId;
}

inline unsigned long IndexedPNG::getWidth() const {
	return _texWidth;
}

inline unsigned long IndexedPNG::getHeight() const {
	return _texHeight;
}
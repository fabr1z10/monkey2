#include "indexedpng.h"
#include "error.h"
#include "png.h"
#include <iostream>
#include <cstring>

IndexedPNG::IndexedPNG(const std::string& fileName) : _data(nullptr) {
	unsigned char sig[8];
	FILE* infile;
	infile = fopen(fileName.c_str(), "rb");
	M_Assert(infile != nullptr, "File " + fileName + " does not exist.");
	/* first do a quick check that the file really is a PNG image; could
	 * have used slightly more general png_sig_cmp() function instead */
	fread(sig, 1, 8, infile);
	if (!png_check_sig(sig, 8)) {
		GLIB_FAIL("File " + fileName + ": bad signature (not a PNG file).")/* bad signature */
	}
	auto png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
		exit(1);   /* out of memory */
	auto info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		exit(1);   /* out of memory */
	}
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		exit(1);
	}
	png_init_io(png_ptr, infile);
	png_set_sig_bytes(png_ptr, 8);  /* we already read the 8 signature bytes */

	png_read_info(png_ptr, info_ptr);  /* read all PNG info up to image data */
	_texWidth = png_get_image_width(png_ptr, info_ptr);
	_texHeight = png_get_image_height(png_ptr, info_ptr);
	_size = _texWidth * _texHeight;
	//_invWidth = 1.f / _texWidth;
	//_invHeight = 1.f / _texHeight;
	auto bitDepth = (int) png_get_bit_depth(png_ptr, info_ptr);
	auto colorType = (int) png_get_color_type(png_ptr, info_ptr);
	std::cout << " -- width: " << _texWidth << std::endl;
	std::cout << " -- height: " << _texHeight << std::endl;
	std::cout << " -- bit depth: " << bitDepth << std::endl;
	std::cout << " -- color type: " << colorType << std::endl;
	M_Assert(colorType == 3, (fileName + " is not an indexed PNG.").c_str());

	auto _row_pointers = new png_bytep[_texHeight];
	unsigned int bytesPerRow = png_get_rowbytes(png_ptr, info_ptr);
	auto _pixels = new unsigned char[bytesPerRow * _texHeight];
	_data = new unsigned char[_texWidth * _texHeight];

	for (int i=0; i < _texHeight; i++) {
		_row_pointers[i] = _pixels + i*bytesPerRow;
	}

	auto pixel_per_byte = 8 / bitDepth;
	unsigned char initSelector;
	switch (bitDepth) {
		case 1:
			initSelector = 0x80;
			break;
		case 2:
			initSelector = 0xC0;
			break;
		case 4:
			initSelector = 0xF0;
			break;
		case 8:
			initSelector = 0xFF;
			break;
	}
	png_read_image(png_ptr, _row_pointers);
	int k = 0;
	int pixel_per_row = 0;
	for (int i = 0; i < bytesPerRow * _texHeight; ++i) {
		// read byte
		unsigned char selector = initSelector;
		// std::cout << " current byte: " << std::hex << (int) _pixels[i] << "\n";
		for (int j = 0; j < pixel_per_byte; ++j) {
			auto pixel = (_pixels[i] & selector) >> (8 - bitDepth * (j + 1));
			_data[k++] = pixel;
			// std::cout << "added pix: " << (int) pixel << "\n";
			pixel_per_row++;
			if (pixel_per_row >= _texWidth) {
				pixel_per_row = 0;
				break;
			}
			selector >>= bitDepth;
		}
	}

	// clean up
	delete [] _row_pointers;
	delete [] _pixels;

	/****************************
	 * PALETTE
	 */
	png_colorp palette = nullptr;
	png_bytep trans_alpha = nullptr;
	int num_palette = 0;

	int num_trans = 0;
	if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette) != PNG_INFO_PLTE) {
		GLIB_FAIL("File " << fileName << ": Failed to get PNG palette.");
	}
	// Get alpha values (optional)
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, nullptr);
	}
	// Allocate RGBA palette
	const int PALETTE_SIZE = 256 * 4;
	_pal = new unsigned char[PALETTE_SIZE];
	std::memset(_pal, 0, PALETTE_SIZE);  // This sets all bytes in pal to 0

	// Fill RGBA values
	for (int i = 0; i < num_palette; ++i) {
		_pal[i * 4 + 0] = palette[i].red;
		_pal[i * 4 + 1] = palette[i].green;
		_pal[i * 4 + 2] = palette[i].blue;
		unsigned char alpha;
		if (i < num_trans) {
			alpha = trans_alpha[i];  // transparency from tRNS
		} else {
			alpha = 255;  // fully opaque
		}
		_pal[i * 4 + 3] = alpha;
	}

}

IndexedPNG::~IndexedPNG() {
	if (_data != nullptr) {
		delete[] _data;
	}
}

unsigned char IndexedPNG::getPixel(int x, int y) {
	return _data[(_texHeight-1-y) * _texWidth + x];
}

void IndexedPNG::load() {
	glGenTextures (1, &_texId);
	glBindTexture (GL_TEXTURE_2D, _texId);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_R8UI , _texWidth, _texHeight, 0,  GL_RED_INTEGER, GL_UNSIGNED_BYTE, _data);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// generate the pal tex
	glGenTextures(1, &_palId);
	glBindTexture(GL_TEXTURE_2D, _palId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &_pal[0]);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}
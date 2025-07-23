//#include "quadbatch.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//#include "error.h"
//#include "game.h"
//#include "png.h"
//
//
//uint32_t PaletteQuadBatch::packRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
//	return (static_cast<uint32_t>(r) << 24) |
//		   (static_cast<uint32_t>(g) << 16) |
//		   (static_cast<uint32_t>(b) << 8)  |
//		   static_cast<uint32_t>(a);
//}
//
//void PaletteQuadBatch::unpackRGBA(uint32_t color, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) {
//	r = (color >> 24) & 0xFF;
//	g = (color >> 16) & 0xFF;
//	b = (color >> 8)  & 0xFF;
//	a = color & 0xFF;
//}
//
//PaletteQuadBatch::PaletteQuadBatch(int size, int cam, const std::string &texture, const YAML::Node& pals) :
//	Batch<primitives::QuadPalette>(size, cam) {
//
//
//
//	int num_palettes = 1 + ((pals.IsDefined() && !pals.IsNull()) ? pals.size() : 0);
//
//	std::cout << " -- Number of palettes: " << num_palettes << std::endl;
//	auto file = Game::instance().getWorkingDirectory() + "/assets/" + texture;
//	unsigned char sig[8];
//	FILE* infile;
//	infile = fopen(file.c_str(), "rb");
//
//	/* first do a quick check that the file really is a PNG image; could
//	 * have used slightly more general png_sig_cmp() function instead */
//	fread(sig, 1, 8, infile);
//	if (!png_check_sig(sig, 8))
//		GLIB_FAIL("File " + texture + ": bad signature (not a PNG file).")/* bad signature */
//	//fprintf(stderr, "all ok\n");
//	auto png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
//	if (!png_ptr)
//		exit(1);   /* out of memory */
//
//
//	auto info_ptr = png_create_info_struct(png_ptr);
//	if (!info_ptr) {
//		png_destroy_read_struct(&png_ptr, NULL, NULL);
//		exit(1);   /* out of memory */
//	}
//
//	//    /* we could create a second info struct here (end_info), but it's only
//	//     * useful if we want to keep pre- and post-IDAT chunk info separated
//	//     * (mainly for PNG-aware image editors and converters) */
//	//
//	//
//	//    /* setjmp() must be called in every function that calls a PNG-reading
//	//     * libpng function */
//	//
//	if (setjmp(png_jmpbuf(png_ptr))) {
//		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
//		exit(1);
//	}
//
//
//	png_init_io(png_ptr, infile);
//	png_set_sig_bytes(png_ptr, 8);  /* we already read the 8 signature bytes */
//
//	png_read_info(png_ptr, info_ptr);  /* read all PNG info up to image data */
//	//png_
////
////
////    /* alternatively, could make separate calls to png_get_image_width(),
////     * etc., but want bit_depth and color_type for later [don't care about
////     * compression_type and filter_type => NULLs] */
////
//	_texWidth = png_get_image_width(png_ptr, info_ptr);
//	_texHeight = png_get_image_height(png_ptr, info_ptr);
//	_invWidth = 1.f / _texWidth;
//	_invHeight = 1.f / _texHeight;
//	auto bitDepth = (int) png_get_bit_depth(png_ptr, info_ptr);
//	auto colorType = (int) png_get_color_type(png_ptr, info_ptr);
//	M_Assert(colorType == 3, (file + " is not an indexed PNG.").c_str());
//
//	std::cout << " -- width: " << _texWidth << std::endl;
//	std::cout << " -- height: " << _texHeight << std::endl;
//	std::cout << " -- bit depth: " << bitDepth << std::endl;
//	std::cout << " -- color type: " << colorType << std::endl;
//	auto _row_pointers = new png_bytep[_texHeight];
//	unsigned int bytesPerRow = png_get_rowbytes(png_ptr, info_ptr);
//	auto _pixels = new unsigned char[bytesPerRow * _texHeight];
//	auto data = new unsigned char[_texWidth * _texHeight];
//
//	for (int i=0; i < _texHeight; i++)
//	{
//		_row_pointers[i] = _pixels + i*bytesPerRow;
//	}
//
//	auto pixel_per_byte = 8 / bitDepth;
//	unsigned char initSelector;
//	switch (bitDepth) {
//		case 1:
//			initSelector = 0x80;
//			break;
//		case 2:
//			initSelector = 0xC0;
//			break;
//		case 4:
//			initSelector = 0xF0;
//			break;
//		case 8:
//			initSelector = 0xFF;
//			break;
//	}
//
//
//
//	png_read_image(png_ptr, _row_pointers);
//	int k = 0;
//	int pixel_per_row = 0;
//	for (int i = 0; i < bytesPerRow * _texHeight; ++i) {
//		// read byte
//		unsigned char selector = initSelector;
//		//std::cout << " current byte: " << std::hex << (int) _pixels[i] << "\n";
//		for (int j = 0; j < pixel_per_byte; ++j) {
//			auto pixel = (_pixels[i] & selector) >> (8 - bitDepth * (j + 1));
//			data[k++] = pixel;
//			//std::cout << "added pix: " << (int) pixel << "\n";
//			pixel_per_row++;
//			if (pixel_per_row >= _texWidth) {
//				pixel_per_row = 0;
//				break;
//			}
//			selector >>= bitDepth;
//		}
//
//	}
////	for (size_t i = 0; i < 10; i++) {
////		std::cout << (int)_data[i] << ", ";
////	}
//
//	// now read palette
//	png_colorp palette = nullptr;
//	png_bytep trans_alpha = nullptr;
//	int num_palette = 0;
//	int num_trans = 0;
//	if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette) != PNG_INFO_PLTE) {
//		GLIB_FAIL("File " << file << ": Failed to get PNG palette.");
//		// handle error
//	}
//
//	// Get alpha values (optional)
//	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
//		png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, nullptr);
//	}
//
//
//	// Allocate RGBA palette
//	const int PALETTE_SIZE = 256 * 4;
//	auto pal = new unsigned char[PALETTE_SIZE];
//	std::memset(pal, 0, PALETTE_SIZE);  // This sets all bytes in pal to 0
//
//	// map color to index
//	std::unordered_map<uint32_t, int> paletteMap;
//	// Fill RGBA values
//	for (int i = 0; i < num_palette; ++i) {
//		pal[i * 4 + 0] = palette[i].red;
//		pal[i * 4 + 1] = palette[i].green;
//		pal[i * 4 + 2] = palette[i].blue;
//		unsigned char alpha;
//		if (i < num_trans) {
//			alpha = trans_alpha[i];  // transparency from tRNS
//		} else {
//			alpha = 255;  // fully opaque
//		}
//		pal[i * 4 + 3] = alpha;
//		// now map hex to index
//		paletteMap[packRGBA(palette[i].red, palette[i].green, palette[i].blue, alpha)] = i;
//	}
//	// now store other palettes! colors that are not changed stay the same
//	// First, copy the palette to other palettes
//	for (int i = 0; i < num_palettes; ++i) {
//		std::memcpy(&pal[i * PALETTE_SIZE], &pal[0], PALETTE_SIZE);
//	}
//	// now replace colors according to input
//	int palIndex = 1;
//	for (const auto& additionalPalette : pals) {
//		int offset = palIndex * PALETTE_SIZE;
//		if (additionalPalette.IsMap()) {
//			for (YAML::const_iterator it = additionalPalette.begin(); it != additionalPalette.end(); ++it) {
//				auto key = it->first.as<uint32_t>();
//				auto value = it->second.as<uint32_t>(); // or use `.Scalar()` if unsure
//				auto it2 = paletteMap.find(key);
//				if (it2 == paletteMap.end()) {
//					delete [] _pixels;
//					delete [] _row_pointers;
//					delete [] data;
//					delete [] pal;
//					GLIB_FAIL("Palette error; don't know color " << std::hex << key);
//				}
//				int index = offset + 4 * it2->second;
//				unpackRGBA(value, pal[index], pal[index+1], pal[index+2], pal[index+3]);
//			}
//		}
//		palIndex++;
//
//	}
//
//
//	std::cout << "---\n";
//	for (int n = 0; n < num_palettes; ++n) {
//		for (int i = 0; i < 1024; ++i) {
//			std::cout << (int) pal[1024*n+i] << ", ";
//		}
//		std::cout << " # end of palette " << n << "\n";
//	}
//
//	glGenTextures (1, &_texId);
//	glBindTexture (GL_TEXTURE_2D, _texId);
//	glTexImage2D (GL_TEXTURE_2D, 0, GL_R8UI , _texWidth, _texHeight, 0,  GL_RED_INTEGER, GL_UNSIGNED_BYTE, data);
//	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//	// generate the pal tex
//	glGenTextures(1, &_palId);
//	glBindTexture(GL_TEXTURE_2D, _palId);
//	//glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pal[0]);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, num_palettes, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pal[0]);
//	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	delete [] _pixels;
//	delete [] _row_pointers;
//	delete [] data;
//	delete [] pal;
//}
//void QuadBatch::configure() {
//    Batch<primitives::Quad>::configure();
//
//
//}
//
//int QuadBatch::addTexture(const std::string &filePath) {
//
//
//
//
//    auto it = _texId.find(filePath);
//    if (it != _texId.end()) {
//        return it->second;
//    }
//
//    int width, height, nrChannels;
//    auto assetFile = Game::instance().getWorkingDirectory() + "/assets/" + filePath;
//    unsigned char* data = stbi_load(assetFile.c_str(), &width, &height, &nrChannels, 0);
//    if (!data) {
//        GLIB_FAIL("Failed to load texture " + assetFile);
//    }
//    if (_texCount == 0) {
//        _texWidth = width;
//        _texHeight = height;
//		_invWidth = 1.f / _texWidth;
//		_invHeight = 1.f / _texHeight;
//        // if this is the 1st texture, generate gl stuff
//*        glGenTextures(1, &_textureArray);
//        glBindTexture(GL_TEXTURE_2D_ARRAY, _textureArray);
//
//        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, _texWidth, _texHeight, _maxTextures);
//
//        // Set texture parameters
//        glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameterf (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//        glTexParameterf (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    }
//
//
//    std::cout << " -- loaded " << filePath << " (" << width << ", " << height << ", " << nrChannels << ")\n";
//    // Make sure the texture fits the array size (image should match the array dimensions)
//    if (width != _texWidth || height != _texHeight) {
//        stbi_image_free(data);
//        GLIB_FAIL( "Texture size does not match the texture array size!");
//    }
//
//    // Upload texture to the texture array
//    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, _texCount, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
//
//    stbi_image_free(data);
//    _texId[filePath] = _texCount;
//    // Increment the texture count
//    return _texCount++;
//}
//
//void QuadBatch::setupUniforms(Shader *s) {
//    IBatch::setupUniforms(s);
//    // Activate texture unit 0
//    glActiveTexture(GL_TEXTURE0);
//
//    // Bind the texture array to texture unit 0
//    glBindTexture(GL_TEXTURE_2D_ARRAY, _textureArray);
//    GLint boundTex = 0;
//    glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &boundTex);
//    //std::cout << "Bound texture array ID: " << boundTex << " (should be " << _textureArray << ")" << std::endl;
//    // Set the uniform to use texture unit 0 (where we bound the texture array)
//    GLuint texLocation = glGetUniformLocation(s->getProgId(), "textureArray");
//    glUniform1i(texLocation, 0);
//
//}
//
//void PaletteQuadBatch::setupUniforms(Shader *s) {
//	IBatch::setupUniforms(s);
//	// Activate texture unit 0
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, _texId);
//	GLuint locDiffuse = glGetUniformLocation(s->getProgId(), "texture_pdiffuse1");
//	glUniform1i(locDiffuse, 0);
//
//	// Activate and bind texture_palette to texture unit 1
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, _palId);
//	GLint locPalette = glGetUniformLocation(s->getProgId(), "texture_palette");
//	glUniform1i(locPalette, 1); // Bind to texture unit 1
//}
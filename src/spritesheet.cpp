#include "spritesheet.h"
#include "png.h"
#include "util.h"
#include "game.h"
#include "font.h"
#include "error.h"
#include "models/tilemodel.h"
#include "yamlcustom.h"


std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> SpriteSheet::sheetCache;

std::shared_ptr<SpriteSheet> SpriteSheet::getSheet(const std::string &file) {
	auto it = sheetCache.find(file);
	if (it == sheetCache.end()) {
		auto sheet = std::make_shared<SpriteSheet>(file);
		sheetCache[file] = sheet;
		return sheet;
	}
	return it->second;
}

void SpriteSheet::clearSheets() {
	SpriteSheet::sheetCache.clear();
}


SpriteSheet::SpriteSheet(const std::string &spritesheetFile) {
	auto assetPath = Game::instance().getWorkingDirectory() + "/assets/";
	try {
		YAML::Node inputData = YAML::LoadFile(assetPath + spritesheetFile);
		auto image = inputData["image"].as<std::string>();
		std::cout << " -- image file: " << image << std::endl;
		auto imageFile = assetPath + image;
		int num_palettes = inputData["palettes"] ? (1+inputData["palettes"].size()) : 1;

		unsigned char sig[8];
		FILE* infile;
		infile = fopen(imageFile.c_str(), "rb");
		M_Assert(infile != nullptr, "File " + imageFile + " does not exist.");
		/* first do a quick check that the file really is a PNG image; could
		 * have used slightly more general png_sig_cmp() function instead */
		fread(sig, 1, 8, infile);
		if (!png_check_sig(sig, 8)) {
			GLIB_FAIL("File " + image + ": bad signature (not a PNG file).")/* bad signature */
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
		//_invWidth = 1.f / _texWidth;
		//_invHeight = 1.f / _texHeight;
		auto bitDepth = (int) png_get_bit_depth(png_ptr, info_ptr);
		auto colorType = (int) png_get_color_type(png_ptr, info_ptr);
		std::cout << " -- width: " << _texWidth << std::endl;
		std::cout << " -- height: " << _texHeight << std::endl;
		std::cout << " -- bit depth: " << bitDepth << std::endl;
		std::cout << " -- color type: " << colorType << std::endl;
		M_Assert(colorType == 3, (image + " is not an indexed PNG.").c_str());

		auto _row_pointers = new png_bytep[_texHeight];
		unsigned int bytesPerRow = png_get_rowbytes(png_ptr, info_ptr);
		auto _pixels = new unsigned char[bytesPerRow * _texHeight];
		auto data = new unsigned char[_texWidth * _texHeight];

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
				data[k++] = pixel;
				// std::cout << "added pix: " << (int) pixel << "\n";
				pixel_per_row++;
				if (pixel_per_row >= _texWidth) {
					pixel_per_row = 0;
					break;
				}
				selector >>= bitDepth;
			}
		}
//		for (size_t i = 0; i < 16; i++) {
//			std::cout << (int)data[i] << ", ";
//		}

		// now read palette
		png_colorp palette = nullptr;
		png_bytep trans_alpha = nullptr;
		int num_palette = 0;

		int num_trans = 0;
		if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette) != PNG_INFO_PLTE) {
			GLIB_FAIL("File " << image << ": Failed to get PNG palette.");
		}
		// Get alpha values (optional)
		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
			png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, nullptr);
		}
		// Allocate RGBA palette
		const int PALETTE_SIZE = 256 * 4;
		auto pal = new unsigned char[num_palettes * PALETTE_SIZE];
		std::memset(pal, 0, num_palettes * PALETTE_SIZE);  // This sets all bytes in pal to 0

		// map color to index
		std::unordered_map<uint32_t, int> paletteMap;
		// Fill RGBA values
		for (int i = 0; i < num_palette; ++i) {
			pal[i * 4 + 0] = palette[i].red;
			pal[i * 4 + 1] = palette[i].green;
			pal[i * 4 + 2] = palette[i].blue;
			unsigned char alpha;
			if (i < num_trans) {
				alpha = trans_alpha[i];  // transparency from tRNS
			} else {
				alpha = 255;  // fully opaque
			}
			pal[i * 4 + 3] = alpha;
			// now map hex to index
			paletteMap[packRGBA(palette[i].red, palette[i].green, palette[i].blue, alpha)] = i;
		}

		// now store other palettes! colors that are not changed stay the same
		// First, copy the palette to other palettes
		for (int i = 1; i < num_palettes; ++i) {
			std::memcpy(&pal[i * PALETTE_SIZE], &pal[0], PALETTE_SIZE);
		}
//		for (int i = 0; i < 1024; ++i) {
//			std::cout << i << " " <<(int) pal[i] << "\n";
//		}
		// now replace colors according to input
		int palIndex = 1;
		for (const auto& additionalPalette : inputData["palettes"]) {
			int offset = palIndex * PALETTE_SIZE;
			for (const auto& color : additionalPalette) {

					auto key = color.first.as<uint32_t>();
					auto value = (color.second.as<uint32_t>() << 8) | 0xFF; // or use `.Scalar()` if unsure
					int u{0};
					if (key != 0) {
						auto it2 = paletteMap.find(key);
						if (it2 == paletteMap.end()) {
							delete[] _pixels;
							delete[] _row_pointers;
							delete[] data;
							delete[] pal;
							GLIB_FAIL("Palette error; don't know color " << std::hex << key);
						}
						u = it2->second;

					}
					int index = offset + 4 * u;
					unpackRGBA(value, pal[index], pal[index + 1], pal[index + 2], pal[index + 3]);

			}
			palIndex++;
		}

		//
		//	}
		for (int n = 0; n < num_palettes; ++n) {
			std::cout << " Palette #" << n << std::endl;
			int o = 1024*n;
			for (int i = 0; i < 1024; i+=4) {
				std::cout << i/4 << ": " << (int) pal[o+i] << ", " << (int) pal[o+i+1] << ", " <<
					(int)pal[o+i+2] << ", " << (int) pal[o+i+3] << "\n";

			}
//				std::cout << (int) pal[1024*n+i] << ", ";
//			}
//			std::cout << " # end of palette " << n << "\n";
		}

		glGenTextures (1, &_texId);
		glBindTexture (GL_TEXTURE_2D, _texId);
		glTexImage2D (GL_TEXTURE_2D, 0, GL_R8UI , _texWidth, _texHeight, 0,  GL_RED_INTEGER, GL_UNSIGNED_BYTE, data);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// generate the pal tex
		glGenTextures(1, &_palId);
		glBindTexture(GL_TEXTURE_2D, _palId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, num_palettes, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pal[0]);
		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, 0);




		loadFonts(inputData);

		loadSprites(inputData);

		delete [] _pixels;
		delete [] _row_pointers;
		delete [] data;
		delete [] pal;
		//	//fprintf(stderr, "all ok\n");
	} catch (const YAML::BadFile& e) {
		GLIB_FAIL("Failed to open " + spritesheetFile);
	}

}

void SpriteSheet::setupUniforms(Shader *s) {
	// Activate texture unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texId);
	GLint locDiffuse = glGetUniformLocation(s->getProgId(), "texture_pdiffuse1");
	glUniform1i(locDiffuse, 0);

	// Activate and bind texture_palette to texture unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _palId);
	GLint locPalette = glGetUniformLocation(s->getProgId(), "texture_palette");
	glUniform1i(locPalette, 1); // Bind to texture unit 1

}

void SpriteSheet::loadFonts(const YAML::Node & node) {
	if (node["fonts"]) {
		for (const auto& font : node["fonts"]) {
			auto fontName = font.first.as<std::string>();
			auto f = std::make_shared<Font>(font.second, _texWidth, _texHeight);
			_fonts[fontName] = f;
		}
	}
}

void SpriteSheet::loadSprites(const YAML::Node & node) {
	if (node["sprites"]) {
		_sprites = node["sprites"];
		_defaultTicks = node["default_ticks"].as<int>(10);
	}
}

std::shared_ptr<models::TileMap> SpriteSheet::makeSprite(int batch, const std::string &id) {
	auto spriteSource = _sprites[id];
	std::unordered_map<std::string, int> quadMap;
	if (!spriteSource) {
		GLIB_FAIL(" ** Error! Don't have sprite: " << id << std::endl);
		exit(1);
	} else {
		auto quadCount = spriteSource["size"].as<int>();
		auto model = std::make_shared<models::TileMap>(batch, quadCount);
		for (const auto& q : spriteSource["quads"]) {
			// read quad
			auto coords = q["coords"].as<glm::ivec4>();
			auto anchor = q["anchor"].as<glm::vec2>(glm::vec2(0.f));
			bool flipx = q["flipx"].as<bool>(false);
			bool flipy = q["flipy"].as<bool>(false);
			auto scale = q["scale"].as<glm::vec2>(glm::vec2(1.f));

			models::TileMap::QuadInfo info;
			info.tx0 = static_cast<float>(coords[0]) / _texWidth;
			info.ty0 = static_cast<float>(coords[1])/ _texHeight;
			info.tx1 = info.tx0 + static_cast<float>(coords[2]) / _texWidth;
			info.ty1 = info.ty0 + static_cast<float>(coords[3]) / _texHeight;
			if (flipx) std::swap(info.tx0, info.tx1);
			if (flipy) std::swap(info.ty0, info.ty1);

			info.x = -anchor.x;
			info.y = -anchor.y;
			info.sizex = coords[2] * scale[0];
			info.sizey = coords[3] * scale[1];
			info.pal = q["pal"].as<int>(0);
			info.bones = q["bones"].as<std::vector<glm::vec2>>(std::vector<glm::vec2>());
			for (auto& b : info.bones) {
				b += glm::vec2(info.x, info.y);
			}

			//info.name = q["name"].as<std::string>("");
			int qId = model->addQuad(info);
			if (q["name"]) {
				quadMap[q["name"].as<std::string>()] = qId;
			}
		}
		for (const auto& a : spriteSource["animations"]) {
			auto name = a.first.as<std::string>();
			models::Animation anim;
			for (const auto& frame : a.second["frames"]) {
				models::Frame f(frame["ticks"].as<int>(_defaultTicks));
				M_Assert(frame["q"].size() == quadCount, std::string("Sprite " + id + " should have " + std::to_string(quadCount) + " quads per frame."));
				for (const auto& q : frame["q"]) {
					int qid = q["ref"] ? quadMap[q["ref"].as<std::string>()] : q["id"].as<int>();
					models::FrameQuad qu(qid);
					qu.position = q["position"].as<glm::vec2>(glm::vec2(0.f));
					qu.flipx = q["flipx"].as<bool>(false);
					qu.flipy = q["flipy"].as<bool>(false);
					qu.scale = q["scale"].as<glm::vec2>(glm::vec2(1.f));
					qu.layer = q["layer"].as<int>(0);
					f.quads.push_back(qu);
				}
				anim.addFrame(f);
				//anim.addFrame(frame["q"].as<std::vector<int>>(), frame["ticks"].as<int>(_defaultTicks));
			}
			model->addAnimation(name, anim);
		}
		return model;
	}
}
#pragma once

#include "../model.h"
#include "../primitives/vertex.h"

namespace models {
	struct FrameQuad {

		FrameQuad(int id) : id(id) {}

		int id;
		glm::vec2 position = glm::vec2(0.f);
		bool flipx = false;
		bool flipy = false;
		glm::vec2 scale = glm::vec2(1.f, 1.f);
		int layer = 0;
	};

	struct Frame {
		Frame(int ticks = 0) : ticks(ticks) {}
		std::vector<FrameQuad> quads;
		int ticks;
	};

	class Animation {
	public:
		Animation() = default;

		void addFrame(const Frame& frame);

		int getFrameCount() const;

		const Frame& getFrame(int) const;

		void setLoopFrame(int);

		int getLoopFrame() const;
	private:
		std::vector<Frame> _frames;
		int _loopFrame{};
	};

	inline void Animation::setLoopFrame(int value) {
		_loopFrame = value;
	}

	inline int Animation::getLoopFrame() const {
		return _loopFrame;
	}

	inline int Animation::getFrameCount() const {
		return _frames.size();
	}

	inline const Frame &Animation::getFrame(int id) const {
		return _frames[id];
	}


	class TileMap : public Model<VertexPalette> {
	public:
		struct QuadInfo {
			float tx0 = 0, tx1 = 0, ty0 = 0, ty1 = 0;
			float sizex = 0, sizey = 0;
			float x = 0, y=0;
			int pal = 0;
			std::vector<glm::vec2> bones;

			//std::string name;
		};

		TileMap(int batch, int n);

		void addQuad(int x, int y, int width, int height, int pal = 0,
					 glm::vec2 pos = glm::vec2(0.f), glm::vec2 scale = glm::vec2(1.f));

		int addQuad(const TileMap::QuadInfo& info);

		glm::vec2 getBonePosition(int boneId) const;
		// tick count = 0 means frame doesn't change
		//void addFrame(const std::vector<int>&, int tickCount);

		void addAnimation(const std::string& id, const Animation& frames);

		bool hasAnimation(const std::string& anim) const;

		void updateImpl() override;

		void draw() override;

		void setEndOfLoop(pybind11::function f);

		std::string getAnimation() override;

		void setAnimation(const std::string&) override;

		void setPalette(int id);

		void setOffset(glm::vec2);

		glm::vec2 getOffset() const;

		void setAutoDraw(bool);

	private:
		int _quadCount;
		int _currentFrame;
		std::string _currentAnimation;
		int _currentTick;
		glm::vec2 _offset;
		bool _autoDraw;
		pybind11::function _endOfLoop;
		bool _firstCallToSet;





		std::vector<QuadInfo> _quads;
		std::unordered_map<std::string, Animation> _animations;
		glm::ivec2 _texSize;
	};

	inline glm::vec2 TileMap::getOffset() const {
		return _offset;
	}

	inline void TileMap::setAutoDraw(bool value) {
		_autoDraw = value;
	}
	inline std::string TileMap::getAnimation() {
		return _currentAnimation;
	}
}


#ifndef TILESPRITE_H
#define TILESPRITE_H

#include "../compatibility.h"
#include "Sprite.h"
#include "TextureAtlas.h"

class TileSprite : public SpriteBase
{
public:
	struct Animation
	{
		~Animation() {
			if(frames) delete [] frames;
		}

		struct Frame
		{
			int tileId;
			int delay;
		} *frames;
		int framesCount;
		unsigned int loop;
	};

	static TileSprite * create(const TextureAtlas::Loader atlasLoader, float x, float y, float z);
	~TileSprite();

	void setTileId(int id);
	int getTileId() const;

	void animate(const Animation& animation);
	void replaceAnimation(const Animation& animation);
	void clearAnimation();

	void draw();

protected:
	int _tileId;
	Animation _animation;
	int _currentFrameId;
	Timestamp _lastFrameTime;

	const TextureAtlas *_texture;

	TileSprite(const TextureAtlas *atlas, float x, float y, float z);
};

#endif // TILESPRITE_H

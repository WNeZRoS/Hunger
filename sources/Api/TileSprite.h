#ifndef TILESPRITE_H
#define TILESPRITE_H

#include "Sprite.h"
#include "TextureAtlas.h"

class TileSprite : public Sprite
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
		int moveSpeed;
		struct { int x, y; } moveTo;
	};

	static TileSprite * create(const TextureAtlas *atlas, float x, float y, float z);
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
	unsigned long long _lastFrameTime;

	const TextureAtlas *_texture;

	TileSprite(const TextureAtlas *atlas, float x, float y, float z);
};

#endif // TILESPRITE_H

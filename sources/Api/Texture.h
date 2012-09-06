#ifndef TEXTURE_H
#define TEXTURE_H

class TextureAtlas;
class TextureManager;

class Texture
{
public:
	typedef const char * Name;

	virtual void activate() const = 0;
	virtual void deactivate() const = 0;

	int getWidth() const;
	int getHeight() const;

	friend class TextureAtlas;
	friend class TextureManager;

protected:
	int _width;
	int _height;
	mutable TextureAtlas *_atlas;
	mutable int _atlasUsage;

	Texture();
	virtual ~Texture();

	TextureAtlas * toAtlas(int rowSize, int columnSize) const;
	void atlasUnload() const;
};

#endif // TEXTURE_H

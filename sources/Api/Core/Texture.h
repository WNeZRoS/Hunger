#ifndef TEXTURE_H
#define TEXTURE_H

class TextureAtlas;
class TextureManager;

class Texture
{
public:
	enum ImageType {
		RGB, RGBA, ALPHA, LUMINANCE, LUMINANCE_ALPHA
	};

	virtual ~Texture();

	typedef const char * Name;

	virtual void activate() const = 0;
	virtual void deactivate() const = 0;
	virtual bool isValidTexture() const = 0;

	int getWidth() const;
	int getHeight() const;

	virtual void setSegment(const unsigned char *bytes, int x, int y, int width, int height, ImageType type) = 0;

	friend class TextureAtlas;
	friend class TextureManager;

protected:
	int _width;
	int _height;
	mutable TextureAtlas *_atlas;
	mutable int _atlasUsage;

	Texture();

	TextureAtlas * toAtlas(int rowSize, int columnSize) const;
	void atlasUnload() const;
};

#endif // TEXTURE_H

#ifndef TEXTURE_H
#define TEXTURE_H

class TextureAtlas;

class Texture {
public:
	virtual ~Texture() = 0;

	virtual void activate() const = 0;
	virtual void deactivate() const = 0;

	int getWidth() const { return _width; }
	int getHeight() const { return _height; }

	virtual TextureAtlas * toAtlas(int rowSize, int columnSize) const = 0;

protected:
	int _width;
	int _height;
};

#endif // TEXTURE_H

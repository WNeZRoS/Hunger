#ifndef ENTITY_H
#define ENTITY_H

class Entity {
public:
	virtual ~Entity() = 0;

	virtual void draw() const = 0;
	virtual void setPosition(int x, int y) { _x = x; _y = y; }
	virtual void getPosition(int& x, int& y) const { x = _x; y = _y; }

protected:
	int _x, _y, _z;
};

#endif // ENTITY_H

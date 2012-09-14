#ifndef CONTROL_H
#define CONTROL_H

#include <vector>

class Controller
{
public:
	virtual ~Controller() = 0;
};

class Hud
{
public:
	virtual ~Hud() = 0;

	virtual void draw() const = 0;
	virtual bool isVisible() const = 0;

	virtual bool start(int x, int y, int pointer) = 0;
	virtual void update(int x, int y, int pointer) = 0;
	virtual void end(int pointer) = 0;
};

class Control
{
public:

	struct Event;
	typedef void (Controller::*CallBackMethod)(const Event& event, int x, int y);
	typedef void (*CallBackFunction)(const Event& event, int x, int y);

	union CallBack
	{
		CallBackMethod m;
		CallBackFunction f;
	};

	enum KeyState
	{
		STATE_NO_PRESSED,
		STATE_PRESSED,
		STATE_DOWN,
		STATE_UP,
		STATE_MOUSE_MOVE,
		STATE_ANY // No contains STATE_MOUSE_MOVE
	};

	enum Keys
	{
		NO_KEY = 0,
		MOUSE_LEFT = 1,     // Pointer 1
		MOUSE_MIDDLE = 2,   // Pointer 2
		MOUSE_RIGHT = 3,    // Pointer 3
		MOUSE_BUTTON_4 = 4, // Pointer 4
		MOUSE_BUTTON_5 = 5, // Pointer 5

		KEY_LEFT = 37,
		KEY_UP = 38,
		KEY_RIGHT = 39,
		KEY_DOWN = 40,
		KEY_ACTION = 32,
		KEY_EXIT = 27
	};

	struct Event
	{
		KeyState state;
		Keys key;
		Controller *instance;
		CallBack callback;
	};

	static const int MAX_POINTERS = 5;

	static Control& instance();
	~Control();

	void idle();
	virtual void drawHud() const;

	int getMouseX(int pointer = MOUSE_LEFT);
	int getMouseY(int pointer = MOUSE_LEFT);
	void setMouseX(int x, int pointer = MOUSE_LEFT);
	void setMouseY(int y, int pointer = MOUSE_LEFT);

	bool isKeyPressed(Keys key);

	void addEvent(const Event& event);
	void addEvent(KeyState state, Keys key, const CallBackFunction& callback);
	void addEvent(KeyState state, Keys key, const Controller *instance, const CallBackMethod& callback);
	void removeEvent(const Event& event);
	void removeEvent(Keys key, KeyState state = STATE_ANY);
	void removeEvent(Controller *controller);

	void addHud(const Hud *hud);
	void removeHud(const Hud *hud);

	void keyboardEvent(KeyState state, Keys key);
	void mouseEvent(KeyState state, Keys key, int x, int y);
	void mouseMoveEvent(int x, int y, int pointer = MOUSE_LEFT);

	void setMultiPointerMode(bool multi);

protected:
	Control();

	void callCallBack(const Controller *instance, CallBack callback, KeyState state, Keys key, int x, int y) const;

	std::vector<Event> _events;
	std::vector<Hud*> _huds;

	int _mouseX[MAX_POINTERS], _mouseY[MAX_POINTERS];
	KeyState _keys[256];
	unsigned long long _lastIdleTime;
	bool _multiPointer;

	//static Control _Control;
};

#endif // CONTROL_H

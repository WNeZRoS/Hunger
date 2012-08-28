#ifndef CONTROL_H
#define CONTROL_H

#include <vector>

class Controller
{
public:
	virtual ~Controller() = 0;
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
		STATE_PRESS,
		STATE_MOUSE_MOVE,
		STATE_ANY
	};

	enum Keys
	{
		NO_KEY = 0,
		MOUSE_LEFT = 1,
		MOUSE_MIDDLE = 2,
		MOUSE_RIGHT = 3,

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

	static Control& instance();
	~Control();

	void idle();
	virtual void drawHud() const;

	int getMouseX();
	int getMouseY();

	bool isKeyPressed(Keys key);

	void addEvent(const Event& event);
	void addEvent(KeyState state, Keys key, const CallBackFunction& callback);
	void addEvent(KeyState state, Keys key, const Controller *instance, const CallBackMethod& callback);
	void removeEvent(const Event& event);
	void removeEvent(Keys key, KeyState state = STATE_ANY);

	void keyboardEvent(KeyState state, Keys key);
	void mouseEvent(KeyState state, Keys key);
	void mouseMoveEvent(int x, int y);

protected:
	Control();

	void callCallBack(const Event& event, int x, int y) const;

	std::vector<Event> _events;
	int _mouseX, _mouseY;
	KeyState _keys[256];
	unsigned long long _lastIdleTime;

	static Control _Control;
};

#endif // CONTROL_H

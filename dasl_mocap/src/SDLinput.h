#include "SDL/SDL.h"

class Joystick{
public:
	Joystick();
	virtual ~Joystick();
	int axis1_, axis2_,axis3_,axis4_,axis5_,axis6_;
	int button1_,button2_,button3_,button4_,button5_,button6_;

	void Refresh();
private:
	SDL_Joystick *joystick;
};

class Keyboard{
public:
	Keyboard();
	virtual ~Keyboard();
	char key;
	SDL_Event keyevent;
	char Refresh();
};

/*
 * Joystick.cpp
 *
 *  Created on: Nov 16, 2011
 *      Author: dasl
 */

#include "SDLinput.h"

Joystick::Joystick() {
	if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0)
	    {
	        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
	        exit(1);
	    }
	printf("%i joysticks were found.\n\n", SDL_NumJoysticks() );
	printf("The names of the joysticks are:\n");

	for(int i=0; i < SDL_NumJoysticks(); i++ )
	{
		printf("    %s\n", SDL_JoystickName(i));
	}
	joystick = SDL_JoystickOpen(0);
	Joystick::Refresh();
}

Joystick::~Joystick() {
	// TODO Auto-generated destructor stub
}
void Joystick::Refresh()
{
	SDL_JoystickUpdate();
	axis1_ = SDL_JoystickGetAxis(joystick,0)*500/32768;
	axis2_ = SDL_JoystickGetAxis(joystick,1)*10/32768;
	axis3_ = SDL_JoystickGetAxis(joystick,2)*500/32768;
	axis4_ = -SDL_JoystickGetAxis(joystick,3)*500/32768;
	axis5_ = -SDL_JoystickGetAxis(joystick,5);//*500/32768;
	button1_=SDL_JoystickGetButton(joystick,0);
	button2_=SDL_JoystickGetButton(joystick,1);
	button3_= SDL_JoystickGetButton(joystick,2);
	button4_ = SDL_JoystickGetButton(joystick,3);
	button5_ = SDL_JoystickGetButton(joystick,4);
	button6_ = SDL_JoystickGetButton(joystick,5);
}

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard(){
}

char Keyboard::Refresh(){
	switch(keyevent.type)
	{
	    case SDL_KEYDOWN:
	     switch(keyevent.key.keysym.sym)
	      {
	        case SDLK_LEFT:
	          return 1;
	          break;
	        case SDLK_RIGHT:
	          return 2;
	          break;
	        case SDLK_UP:
	          return 3;
	          break;
	        case SDLK_DOWN:
	          return 4;
	          break;
	        default:
	        	return 0;
	        	break;
	      }
	}
}

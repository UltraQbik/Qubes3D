#include "Main.h"


extern Window g_Window(1280, 720, 75);
extern World g_World;


int main()
{
	while (g_Window.getWindow().isOpen())
	{
		// keyboard interation
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			return 0;

		// TODO: camera

		// TODO: multithreading (a good one hopefully)
		debugScreenTest();

		// update the window
		g_Window.onUpdate();
	}

	// end of program
	return 0;
}
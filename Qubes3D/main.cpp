#include <SFML/Graphics.hpp>

#include <stdio.h>
#include "Config.h"
#include "Window.h"
#include "Render.h"
#include "Vector.h"
#include "World.h"
#include "Camera.h"


Window g_Window(480, 360, 75);
World g_World;
Camera g_Camera(FVec3(10.392f));


int main()
{
	// generate world with some cubes in it
	generateDebugWorld(g_World);

	while (g_Window.getWindow().isOpen())
	{
		// call camera update
		if (g_Window.getWindow().hasFocus())
			g_Camera.onUpdate(g_Window.getDelta());

		// TODO: multithreading (a good one hopefully)
		render();
		std::cout << 1.f / g_Window.getDelta() << "\n";

		// update the window
		g_Window.onUpdate();
	}

	// end of program
	return 0;
}
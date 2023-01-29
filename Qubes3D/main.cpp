#include <SFML/Graphics.hpp>

#include <stdio.h>
#include "Config.h"
#include "Window.h"
#include "Render.h"
#include "Vector.h"
#include "World.h"
#include "Camera.h"


Window g_Window(1280, 720, 75);
World g_World;
Camera g_Camera(Vec3<float>(4));


int main()
{
	// generate world with some cubes in it
	generateDebugWorld(g_World);

	while (g_Window.getWindow().isOpen())
	{
		// keyboard interation
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			return 0;

		// call camera update
		g_Camera.onUpdate(g_Window.getDelta());

		// TODO: multithreading (a good one hopefully)
		render();

		// update the window
		g_Window.onUpdate();
	}

	// end of program
	return 0;
}
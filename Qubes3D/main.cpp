#include <iostream>
#include <SFML/Graphics.hpp>

#include "Config.h"
#include "Window.h"
#include "Render.h"
#include "Vector.h"
#include "World.h"
#include "Camera.h"


// TODO: clean up main


Window g_Window(1280, 720, 75);
World g_World;
Camera g_Camera(Vec3<float>(10.392f));


int main()
{
	// generate world with some cubes in it
	generateFlatWorld(g_World, 16);

	while (g_Window.getWindow().isOpen())
	{
		// Camera update
		if (g_Window.getWindow().hasFocus())
			g_Camera.onUpdate(g_Window.getDelta());

		// TODO: multithreading (a good one hopefully)
		renderFullCon();

		// Update the window
		g_Window.onUpdate();
	}

	// end of program
	return 0;
}
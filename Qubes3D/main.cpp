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

	// events
	sf::Event ev;

	while (g_Window.getWindow().isOpen())
	{
        // event polling
        while (g_Window.getWindow().pollEvent(ev)) {
            switch (ev.type) {
            case sf::Event::Closed:
                g_Window.getWindow().close();
                break;
            case sf::Event::KeyPressed:
                if (ev.key.code == sf::Keyboard::Escape)
                    g_Window.getWindow().close();
                break;
            }
        }

		// call camera update
		if (g_Window.getWindow().hasFocus())
			g_Camera.onUpdate(g_Window.getDelta());

		// TODO: multithreading (a good one hopefully)
		render();

		// update the window
		g_Window.onUpdate();
	}

	// end of program
	return 0;
}
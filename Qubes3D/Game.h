#pragma once
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Vector.h"
#include "Render.h"
#include "World.h"


class Camera
{
public:
	Camera();

	Vec3f m_Pos;
	Vec3f m_Dir;
};

class Game
{
public:
	Game();
	~Game();

	void run();
private:
	/// Window
	sf::RenderWindow m_Window;

	float m_FrameDelta;
	std::chrono::steady_clock::time_point m_Timer;

	sf::Texture m_ScreenTexture;
	sf::Sprite m_BufferSprite;
	sf::Uint8* m_ScreenBuffer;

	/// World
	Vec3f m_Sun = Vec3f(-2.f, 3.f, -4.f).norm();
	World m_World;

	/// Player camera
	Camera m_Camera;

	/// Rendering
	Ray castRay(const Vec3f& pos, const Vec3f& dir);
	Vec3f getNormal(const Vec3f& pos);
	Vec3f calculatePixel(float u, float v);
	void renderRange(int start, int end);
};


void generateDebugWorld(World& _world);
void generateFlatWorld(World& _world, const int level);
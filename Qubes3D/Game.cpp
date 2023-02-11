#include <thread>
#include "game.h"
#include "render.h"
#include "amath.h"


Game::Game()
	: m_Window(sf::VideoMode(g_WINDOW_WIDTH, g_WINDOW_HEIGHT), "The Almighty Qubes 3D", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize), m_World(), m_Camera()
{
	// Window
	m_FrameDelta = 0.00001f;
	m_Timer = std::chrono::high_resolution_clock::now();

	m_ScreenBuffer = new sf::Uint8[(uint64_t)g_WINDOW_WIDTH * g_WINDOW_HEIGHT * 4];

	m_ScreenTexture.create(g_WINDOW_WIDTH, g_WINDOW_HEIGHT);
	m_BufferSprite.setTexture(m_ScreenTexture);

	m_Window.setFramerateLimit(g_WINDOW_FPS);
	m_Window.setMouseCursorGrabbed(true);
	m_Window.setMouseCursorVisible(false);

	// World
	generateFlatWorld(m_World, g_CHUNK_SIZE * g_MAP_SIZE / 3);

	// Player camera
	m_Camera.m_Pos.x = g_CHUNK_SIZE * g_MAP_SIZE / 2.f;
	m_Camera.m_Pos.y = g_CHUNK_SIZE * g_MAP_SIZE / 2.f;
	m_Camera.m_Pos.z = g_CHUNK_SIZE * g_MAP_SIZE / 3.f + 1.f;
}

Game::~Game()
{
	m_Window.close();
	delete[] m_ScreenBuffer;
}

void Game::run()
{
	while (true)
	{
		{  /// Player camera update
			// mouse controls
			float mouse_x, mouse_y;

			mouse_x = (float)sf::Mouse::getPosition(m_Window).x / g_WINDOW_WIDTH - 0.5f;
			mouse_y = (float)sf::Mouse::getPosition(m_Window).y / g_WINDOW_HEIGHT - 0.5f;

			sf::Mouse::setPosition(sf::Vector2i(g_WINDOW_WIDTH / 2, g_WINDOW_HEIGHT / 2), m_Window);

			m_Camera.m_Dir.x = std::fmodf(m_Camera.m_Dir.x - mouse_y * g_CAM_SENSITIVITY * m_FrameDelta, 6.2831853f);
			m_Camera.m_Dir.x = clamp(m_Camera.m_Dir.x, -1.5f, 1.5f);
			m_Camera.m_Dir.z = std::fmodf(m_Camera.m_Dir.z - mouse_x * g_CAM_SENSITIVITY * m_FrameDelta, 6.2831853f);

			// keyboard movement
			float movement_speed = g_CAM_SPEED * m_FrameDelta;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				m_Camera.m_Pos.x += std::cosf(m_Camera.m_Dir.z + 1.5707963f) * movement_speed;
				m_Camera.m_Pos.y += std::sinf(m_Camera.m_Dir.z + 1.5707963f) * movement_speed;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				m_Camera.m_Pos.x -= std::cosf(m_Camera.m_Dir.z + 1.5707963f) * movement_speed;
				m_Camera.m_Pos.y -= std::sinf(m_Camera.m_Dir.z + 1.5707963f) * movement_speed;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				m_Camera.m_Pos.x -= std::cosf(m_Camera.m_Dir.z) * movement_speed;
				m_Camera.m_Pos.y -= std::sinf(m_Camera.m_Dir.z) * movement_speed;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				m_Camera.m_Pos.x += std::cosf(m_Camera.m_Dir.z) * movement_speed;
				m_Camera.m_Pos.y += std::sinf(m_Camera.m_Dir.z) * movement_speed;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				m_Camera.m_Pos.z += movement_speed;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
				m_Camera.m_Pos.z -= movement_speed;
			}

			// Check for events (god this looks awful)
			sf::Event ev;
			while (m_Window.pollEvent(ev))
			{
				switch (ev.type)
				{
				case sf::Event::Closed:
					return;
					break;
				case sf::Event::KeyPressed:
					if (ev.key.code == sf::Keyboard::Escape)
						return;
					break;
				case sf::Event::MouseButtonPressed:
					if (ev.mouseButton.button == sf::Mouse::Left)
					{
						Ray ray = castRay(m_Camera.m_Pos, rotateZ(rotateX(Vec3f(0, 1, 0), m_Camera.m_Dir.x), m_Camera.m_Dir.z));
						Vec3f normal = getNormal(ray.fpos);
						Vec3<POS> block_pos = ray.ipos - Vec3<POS>((POS)normal.x, (POS)normal.y, (POS)normal.z);
						m_World.setBlockP(block_pos, 1);
					}
					if (ev.mouseButton.button == sf::Mouse::Right)
					{
						Ray ray = castRay(m_Camera.m_Pos, rotateZ(rotateX(Vec3f(0, 1, 0), m_Camera.m_Dir.x), m_Camera.m_Dir.z));
						m_World.setBlockP(ray.ipos, 0);
					}
				}
			}
		}
		{  /// Window render
			int thread_count = std::thread::hardware_concurrency() / 4;
			std::vector<std::thread> threads;
			threads.reserve(thread_count);

			for (int i = 0; i < thread_count; i++)
				threads.emplace_back(std::thread(&Game::renderRange, this, (int)(g_WINDOW_HEIGHT * ((float)i / thread_count)), (int)(g_WINDOW_HEIGHT * ((float)(i + 1) / thread_count))));

			for (auto& th : threads)
				th.join();
		}
		{  /// Window update
			m_ScreenTexture.update(m_ScreenBuffer);			// update the texture
			m_Window.draw(m_BufferSprite);					// blit the sprite on screen
			m_Window.display();								// display everything

			m_FrameDelta = (std::chrono::high_resolution_clock::now() - m_Timer).count() * 1e-9f;
			m_Timer = std::chrono::high_resolution_clock::now();
		}
	}
}
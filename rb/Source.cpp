#include "raylib.h"

class Projectile
{
private:
	float m_x, m_y, m_radius;
	float m_speed;

public:
	Projectile(float x, float y, float radius, float speed)
		:m_x{ x }, m_y{ y }, m_radius{ radius }, m_speed{ speed }
	{

	}

	Projectile draw()
	{
		DrawCircle(m_x, m_y, m_radius, WHITE);
		return *this;
	}
};

class Player
{
private:
	float m_x, m_y;
	float m_speed;
	float m_width, m_height;
	Projectile projectile{ m_x, m_y - m_height, 5, 150 };

public:
	Player(float x, float y, float speed, float width, float height)
		: m_x{ x }, m_y{ y }, m_speed{ speed }, m_width{width}, m_height{height}
	{

	}

	Player draw()
	{
		DrawTriangle(Vector2{ m_x, m_y - m_height}, Vector2{ m_x - m_width / 2, m_y},
			Vector2{ m_x + m_width / 2, m_y }, BLUE);
		return *this;
	}

	Player move()
	{
		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
		{
			m_x -= m_speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
		{
			m_x += m_speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
		{
			m_y -= m_speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
		{
			m_y += m_speed * GetFrameTime();
		}

		return *this;
	}

	Player shoot()
	{
		if (IsKeyDown(KEY_SPACE))
			projectile.draw();
		
		return *this;
	}
};




int main()
{
	SetConfigFlags(FLAG_FULLSCREEN_MODE);
	InitWindow(GetScreenWidth(), GetScreenHeight(), "Star Conflict");

	Player player{ GetScreenWidth() / 2.0f, GetScreenHeight() * 1.0f - 50, 200, 80, 80 };

	while (!WindowShouldClose())
	{
		player.move();

		BeginDrawing();
		ClearBackground(BLACK);
		player.draw().shoot();
		
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
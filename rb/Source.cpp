#include "raylib.h"
#include <vector>

typedef struct Timer {
	double startTime;   // Start time (seconds)
	double lifeTime;    // Lifetime (seconds)
} Timer;

void StartTimer(Timer* timer, double lifetime)
{
	timer->startTime = GetTime();
	timer->lifeTime = lifetime;
}

bool TimerDone(Timer timer)
{
	return GetTime() - timer.startTime >= timer.lifeTime;
}

double GetElapsed(Timer timer)
{
	return GetTime() - timer.startTime;
}

class Player;

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

	float getY()const { return m_y; }

	Projectile draw()
	{
		DrawCircle(m_x, m_y, m_radius, WHITE);
		return *this;
	}

	Projectile move()
	{
		m_y -= m_speed * GetFrameTime();
		return *this;
	}
};

class Player
{
private:
	float m_x, m_y;
	float m_speed;
	float m_width, m_height;
	std::vector<Projectile> projectile;

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

	Player shoot(float delay, Timer &timer)
	{
		for (size_t i = 0; i < projectile.size(); ++i)
		{
			projectile.at(i).move();
			projectile.at(i).draw();
			if (projectile.at(i).getY() <= 0)
				projectile.erase(projectile.begin() + i);
		}
		
		if (IsKeyDown(KEY_SPACE) && TimerDone(timer))
		{
			projectile.push_back({ m_x, m_y - m_height, 5, 150 });
			StartTimer(&timer, delay);
			
		}
		GetElapsed(timer);
		return *this;
	}

	Player pmove()
	{
		
		return *this;
	}
};




int main()
{
	SetConfigFlags(FLAG_FULLSCREEN_MODE);
	InitWindow(GetScreenWidth(), GetScreenHeight(), "Star Conflict");

	Player player{ GetScreenWidth() / 2.0f, GetScreenHeight() * 1.0f - 50, 200, 80, 80 };

	float shooting_delay = 1.0f;
	Timer shoot_timer{ 0 };
	while (!WindowShouldClose())
	{
		player.move();
		player.shoot(shooting_delay, shoot_timer);
		BeginDrawing();
		ClearBackground(BLACK);
		player.draw();
		
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
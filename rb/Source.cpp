#include "raylib.h"
#include <vector>

bool game_over = false;

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

	Vector2 getVec() const { return { m_x, m_y }; }

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

	/*void PlayerAttacked(Player &player)
	{
		
	}

	void EnemyAttacked()
	{

	}*/
};


class Player
{
private:
	float m_x, m_y;
	float m_speed;
	float m_width, m_height;
	float m_shooting_delay;
	Timer m_shoot_timer;
	std::vector<Projectile> projectile;

public:
	Player(float x, float y, float speed, float width, float height)
		: m_x{ x }, m_y{ y }, m_speed{ speed }, m_width{width}, m_height{height}, m_shooting_delay{0.4f}, m_shoot_timer{0}
	{

	}

	~Player()
	{

	}

	Vector2 getPoint1()const { return { m_x, m_y - m_height }; }
	Vector2 getPoint2()const { return { m_x - m_width / 2, m_y }; }
	Vector2 getPoint3()const { return { m_x + m_width / 2, m_y }; }

	Player draw()
	{
		DrawTriangle(getPoint1(), getPoint2(),
			getPoint3(), BLUE);
		return *this;
	}


	Player move()
	{
		if (m_x - m_width / 2 < 0)
			m_x = m_width / 2;
		if (m_x + m_width / 2 > GetScreenWidth())
			m_x = GetScreenWidth() - m_width / 2;
		if (m_y - m_height < 0)
			m_y = m_height;
		if (m_y > GetScreenHeight())
			m_y = GetScreenHeight();

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
		for (size_t i = 0; i < projectile.size(); ++i)
		{
			projectile.at(i).move();
			projectile.at(i).draw();
			if (projectile.at(i).getY() <= 0)
				projectile.erase(projectile.begin() + i);
		}
		
		if (IsKeyDown(KEY_SPACE) && TimerDone(m_shoot_timer))
		{
			projectile.push_back({ m_x, m_y - m_height, 5, 150 });
			StartTimer(&m_shoot_timer, m_shooting_delay);
			
		}

		GetElapsed(m_shoot_timer);
		return *this;
	}

	/*void playerDeath()
	{
		if (CheckCollisionPointTriangle(getVec(), player.getPoint1(), player.getPoint2(), player.getPoint3()))
		{
			game_over = true;
		}
	}*/

};


class Enemy
{
private:
	float m_x, m_y;
	float m_speed;
	float m_width, m_height;
	float m_shooting_delay;
	Timer m_shoot_timer;
	std::vector<Projectile> projectile;

public:
	Enemy(float x, float y, float speed, float width, float height)
		: m_x{ x }, m_y{ y }, m_speed{ speed }, m_width{ width }, m_height{ height }, m_shooting_delay{0.4f}, m_shoot_timer{0}
	{

	}

	~Enemy()
	{

	}

	Vector2 getPoint1()const { return { m_x - m_width / 2, m_y - m_height };}
	Vector2 getPoint2()const { return  { m_x, m_y };}
	Vector2 getPoint3()const { return { m_x + m_width / 2, m_y - m_height }; }

	Enemy draw()
	{
		DrawTriangle(getPoint1(), getPoint2(),
			getPoint3(), RED);
		return *this;
	}

	Enemy shoot()
	{
		for (size_t i = 0; i < projectile.size(); ++i)
		{
			projectile.at(i).move();
			projectile.at(i).draw();
			if (projectile.at(i).getY() <= 0)
				projectile.erase(projectile.begin() + i);
		}

		if (TimerDone(m_shoot_timer))
		{
			projectile.push_back({ m_x, m_y, 5, -150 });
			StartTimer(&m_shoot_timer, m_shooting_delay);

		}

		GetElapsed(m_shoot_timer);
		return *this;
	}


};


int main()
{
	SetConfigFlags(FLAG_FULLSCREEN_MODE);
	InitWindow(GetScreenWidth(), GetScreenHeight(), "Star Conflict");

	Player player{ GetScreenWidth() / 2.0f, GetScreenHeight() * 1.0f - 50, 200, 80, 80 };
	Enemy enemy{ GetScreenWidth() / 2.0f, 100, 200, 80, 80 } ;

	
	while (!WindowShouldClose())
	{
		if (!game_over)
		{
			player.move();
			player.shoot();
			enemy.shoot();
		}
		else
			DrawText("Game Over", GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f, 30, YELLOW);
		

		BeginDrawing();
		ClearBackground(BLACK);
		player.draw();
		enemy.draw();
		
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
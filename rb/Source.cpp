#include "raylib.h"
#include <vector>

bool game_over = false;
float direction = 1;
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

class Enemies;

class Player
{
private:
	float m_x, m_y;
	float m_speed;
	float m_width, m_height;
	float m_shooting_delay;
	Timer m_shoot_timer;
	std::vector<Projectile> projectile;
	Color m_color;

public:
	Player(float x, float y, float speed, float width, float height, Color color = BLUE)
		: m_x{ x }, m_y{ y }, m_speed{ speed }, m_width{width}, m_height{height}, m_shooting_delay{0.4f}, m_shoot_timer{0}, m_color{color}
	{

	}

	~Player()
	{

	}

	Vector2 getPoint1()const { return { m_x, m_y - m_height }; }
	Vector2 getPoint2()const { return { m_x - m_width / 2, m_y }; }
	Vector2 getPoint3()const { return { m_x + m_width / 2, m_y }; }

	std::vector<Projectile> getProj() { return projectile; }

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

	void playerDeath(Enemies& enemy);

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
	Color m_color;

public:
	Enemy(float x, float y, float speed, float width, float height)
		: m_x{ x }, m_y{ y }, m_speed{ speed }, m_width{ width }, m_height{ height }, m_shooting_delay{0.4f}, m_shoot_timer{0}, m_color{RED}
	{

	}

	~Enemy()
	{

	}

	std::vector<Projectile> getProj() { return projectile; }

	Vector2 getPoint1()const { return { m_x - m_width / 2, m_y - m_height };}
	Vector2 getPoint2()const { return  { m_x, m_y };}
	Vector2 getPoint3()const { return { m_x + m_width / 2, m_y - m_height }; }

	Enemy draw()
	{
		DrawTriangle(getPoint1(), getPoint2(),
			getPoint3(), m_color);
		return *this;
	}

	Enemy move()
	{
		if (m_x < m_width / 2)
			direction = 1;
		
		if (m_x > GetScreenWidth() - m_width / 2)
			direction = -1;
		m_x += m_speed * GetFrameTime() * direction;
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
			projectile.push_back({ m_x, m_y, 5, -90 });
			StartTimer(&m_shoot_timer, m_shooting_delay);

		}

		GetElapsed(m_shoot_timer);
		return *this;
	}

	void death(Player &player)
	{
		for (size_t i = 0; i < player.getProj().size(); ++i)
		{
			if (CheckCollisionPointTriangle(player.getProj().at(i).getVec(), getPoint1(), getPoint2(), getPoint3()))
			{
				m_color = PURPLE ;
			}
		}
		
	}

};

class Enemies
{
private:
	std::vector<Enemy> m_enemy{ {GetScreenWidth() / 2.0f, 100, 200, 80, 80} };

public:

	std::vector<Enemy> getVector()const { return m_enemy; }

	Enemies draw()
	{
		for (size_t i = 0; i < m_enemy.size(); ++i)
		{
			m_enemy.at(i).draw();
		}
		return *this;
	}

	Enemies move()
	{
		for (size_t i = 0; i < m_enemy.size(); ++i)
		{
			m_enemy.at(i).move();
		}
		return *this;
	}

	Enemies shoot()
	{
		for (size_t i = 0; i < m_enemy.size(); ++i)
		{
			m_enemy.at(i).shoot();
		}
		return *this;
	}

	Enemies death(Player& player)
	{
		for (size_t i = m_enemy.size(); i > 0; --i)
		{
			for (size_t j = 0; j < player.getProj().size(); ++j)
			{
				if (CheckCollisionPointTriangle(player.getProj().at(j).getVec(), m_enemy.at(i-1).getPoint1(), m_enemy.at(i-1).getPoint2(), m_enemy.at(i-1).getPoint3()))
				{
					m_enemy.erase(m_enemy.begin() + (i - 1));
					break;
				}
			}
		}
		return *this;
	}
};

void Player::playerDeath(Enemies& enemy)
{
	for (size_t i = 0; i < enemy.getVector().size(); ++i)
	{
		for (size_t i = 0; i < enemy.getVector().at(i).getProj().size(); ++i)
		{
			if (CheckCollisionPointTriangle(enemy.getVector().at(i).getProj().at(i).getVec(), getPoint1(), getPoint2(), getPoint3()))
			{
				m_color = PURPLE;
			}
		}
	}
}

int main()
{
	SetConfigFlags(FLAG_FULLSCREEN_MODE);
	InitWindow(GetScreenWidth(), GetScreenHeight(), "Star Conflict");

	Player player{ GetScreenWidth() / 2.0f, GetScreenHeight() * 1.0f - 50, 200, 80, 80 };
	Enemies enemy/*{ GetScreenWidth() / 2.0f, 100, 200, 80, 80 }*/;

	
	while (!WindowShouldClose())
	{
		if (!game_over)
		{
			player.move();
			player.shoot();
			enemy.shoot();
			enemy.move();
			enemy.death(player);
			/*player.playerDeath(enemy);*/
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
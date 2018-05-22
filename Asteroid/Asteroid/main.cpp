#include <SFML/Graphics.hpp>
#include <iostream> 
#include <cstring>
#include <cstdlib>
#include <math.h>

using namespace sf;

int height = 720, width = 1280;

String levelText ="";
int level = 0;
RenderWindow window(sf::VideoMode(width, height), "Asteroids");

class Players {
private:
	float x,y,angle,speedx,speedy,respawnTime;
	int health;
	bool respawned;
public:
	bool died = false;
	Image image;
	Texture texture;
	String File;
	Sprite sprite;

	Players(String F) {
		File = F;
		x = width / 2.0;
		y = height / 2.0;
		angle = 0;
		speedx = 0;
		speedy = 0;
		respawnTime = 0;
		health = 3;
		respawned = false;
		image.loadFromFile("texture/" + F);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		image.createMaskFromColor(Color(0, 0, 0));
		sprite.setTextureRect(IntRect(0, 0, 19, 28));
	}

	void update(float time) {
		x += speedx*time/50.0;
		y += speedy*time/50.0;
		if (abs(speedx) > 200)speedx = 200.0;
		if (abs(speedy) > 200)speedy = 200.0;
		if (speedx > 0)speedx -= 0.005;
		if (speedx < 0)speedx += 0.005;
		if (speedy > 0)speedy -= 0.005;
		if (speedy < 0)speedy += 0.005;
		sprite.setPosition(x, y);
		if (x > width)x = 0;
		if (x < 0)x = width;
		if (y > height)y = 0;
		if (y < 0)y = height;
		if (respawned) {
			respawnTime += 1;
			sprite.setColor(Color::Green);
		}
		if (respawnTime > 6000) {
			respawnTime = 0;
			respawned = false;
			sprite.setColor(Color::White);
		}
	}

	void boost(float SPEEDX, float SPEEDY) {
		speedx += SPEEDX;
		speedy += SPEEDY;
	}

	void rotate(float ANGLE) {
		angle += ANGLE;
		if (angle > 360)angle -= 360;
		if (angle < 0)angle += 360;
		sprite.setRotation(angle);
	}
	void check(int xa, int ya, int WidthAst, int HeightAst, int size) {
		if ((x < (WidthAst*size + xa) && (x > xa) && (y < (ya + HeightAst * size)) && (y > ya))&&(!respawned))respawn();
	}

	void respawn() {
		y = (height / 2);
		x = (width / 2);
		speedx = 0;
		speedy = 0;
		health -= 1;
		respawned = true;
		if (health == 0) {
			level = 0;
			died = true;
			health = 3;
		}
	}
	float getX() {
		return x;
	}
	float getY() {
		return y;
	}
	float getAngle() {
		return angle;
	}
}player("player.png");

class Asteroids {
private:
	float x, y, speedx, speedy;
	int size, HeightAst=84, WidthAst=89;
	bool status = false;
	int health;
public:

	Image image;
	Texture texture;
	String File;
	Sprite sprite;

	void onCreate(int s, float X, float Y) {
		File = "asteroid.png";
		int Case = rand() % 10;
		if ((X != 0)&&(Y != 0)) {
			x = X + (10 - rand() % 20);
			y = Y + (10 - rand() % 20);
		}
		else {
			switch (Case) {
			case 0:x = -10; y = rand() % 720; break;
			case 1:x = -10; y = rand() % 720; break;
			case 2:x = 1290; y = rand() % 720; break;
			case 3:x = 1290; y = rand() % 720; break;
			case 4:y = -10; x = rand() % 720; break;
			case 5:y = -10; x = rand() % 720; break;
			case 6:y = -10; x = rand() % 720; break;
			case 7:y = 1290; x = rand() % 720; break;
			case 8:y = 1290; x = rand() % 720; break;
			case 9:y = 1290; x = rand() % 720; break;
			}
		}

		speedx = 0 - rand() % 10;
		speedy = 0 - rand() % 10;
		
		size = s;
		image.loadFromFile("texture/" + File);
		//image.loadFromFile("pass.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		image.createMaskFromColor(Color(0, 0, 0));
		sprite.setTextureRect(IntRect(0, 0, WidthAst, HeightAst));
		sprite.setScale(size, size);
		texture.setSmooth(true);
		status = true;
		health = size * 50;
		//health = 1;
	}

	void update(float time) {
		x += speedx * time / 50;
		y += speedy * time / 50;
		sprite.setPosition(x, y);
		if (x > width+WidthAst)x = 0;
		if (x < 0 - WidthAst)x = width;
		if (y > height + HeightAst)y = 0;
		if (y < 0 - HeightAst)y = height;
	}

	void boost(float SPEEDX, float SPEEDY) {
		speedx += SPEEDX;
		speedy += SPEEDY;
	}

	void remove() {
		status = false;
	}

	float getX() {
		return x;
	}
	float getY() {
		return y;
	}
	int getHeight() {
		return HeightAst;
	}
	int getWidth() {
		return WidthAst;
	}
	bool getStatus() {
		return status;
	}
	int getSize() {
		return size;
	}

	void hit() {
		int count = 0;
		if (health > 0)health--;
		else {
			for (int k = 0; k < 20; k++) {
				if (count == 2)break;
				if (size > 1) {
					if (!(asteroids[k].getStatus())) {
						asteroids[k].onCreate(size - 1,getX(), getY());
						count++;
					}
				}
			}
			remove();
		}
	}
}asteroids[20];

String inttostr(int a)
{
	char numb[11] = "0123456789";
	int counter = 1;
	int field[9];
	char retur[10] = "         ";
	for (int i = 0; i <9; i++)
	{
		if ((a / 10)>0)
		{
			field[i] = a;
			a = a / 10;
			counter += 1;
		}
		else { field[i] = a % 10; break; }
	}
	int j;
	j = field[counter - 1];
	retur[0] = numb[j];
	for (int k = 1; k < counter; k++)
	{
		j = field[counter - (k + 1)] - field[counter - k] * 10;
		retur[k] = numb[j];
	}
	return retur;
}

struct {
	float health, angle ,x, y, speedx, speedy;
}fire[20];

void newFire(int i, float x, float y, float angle) {
	fire[i].x = x;
	fire[i].y = y;
	float dx = sin(angle*3.14159265 / 180.0);
	float dy = cos(angle*3.14159265 / 180.0);
	fire[i].speedx = dx;
	fire[i].speedy = -dy;
	fire[i].health = 0;
}

void drawFire() {
	for (int i = 0; i < 20; i++) {
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(fire[i].x , fire[i].y)),
			sf::Vertex(sf::Vector2f(fire[i].x + fire[i].speedx*10, fire[i].y + fire[i].speedy*10))
		};
		window.draw(line, 2, sf::Lines);
	}
}

void checkFire() {
	int x, y, WidthAst, HeightAst;
	for (int k = 0; k < 20; k++) {
		x = asteroids[k].getX();
		y = asteroids[k].getY();
		WidthAst = asteroids[k].getWidth()*asteroids[k].getSize();
		HeightAst = asteroids[k].getHeight()*asteroids[k].getSize();
		if (asteroids[k].getStatus()) {
			for (int i = 0; i < 20; i++) {
				if ((fire[i].x < (WidthAst + x) && (fire[i].x > x) && (fire[i].y < (y + HeightAst)) && (fire[i].y > y))) {
					newFire(i, player.getX(), player.getY(), player.getAngle());
					asteroids[k].hit();
				}
			}
		}
	}
}

void moveFire() {
	for (int i = 0; i < 20; i++) {
		fire[i].x += fire[i].speedx;
		fire[i].y += fire[i].speedy;
		fire[i].health += 1;
		if (fire[i].health > 400)newFire(i, player.getX(), player.getY(), player.getAngle());
		checkFire();
	}
}

int levelUp(int l) {
	l++;
	for (int i = 0; i < l; i++) {
		asteroids[i].onCreate(3, 0 , 0);
	}
	levelText = "Level: " + inttostr(l);
	return l;
}

int checkLevelUp(int l) {
	bool f = true;
	for (int k = 0; k < 20; k++)if(asteroids[k].getStatus())f = false;
	if (f)l = levelUp(l);
	return l;
}

int main()
{
	Clock clock;

	srand(time(NULL));
	
	Font font;
	font.loadFromFile("font/CyrilicOld.ttf");
	Text text(levelText, font, 20);
	text.setStyle(sf::Text::Bold);
	text.setPosition(20, 20);

	for (int i = 0; i < 20; i++) {
		newFire(i, player.getX(), player.getY(), player.getAngle());
	}
		
	level = levelUp(level);

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;

		if (player.died) {
			level = levelUp(level);
			player.died = false;
		}
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::A))player.boost(-0.025, 0.0);
		if (Keyboard::isKeyPressed(Keyboard::S))player.boost(0.0, 0.025);
		if (Keyboard::isKeyPressed(Keyboard::D))player.boost(0.025, 0.0);
		if (Keyboard::isKeyPressed(Keyboard::W))player.boost(0.0, -0.025);
		if (Keyboard::isKeyPressed(Keyboard::J))player.rotate(-0.1);
		if (Keyboard::isKeyPressed(Keyboard::K))player.rotate(0.1);

		moveFire();
		player.update(time);

		for (int i = 0; i < 20; i++) {
			if (asteroids[i].getStatus()) {
				asteroids[i].update(time);
				player.check(asteroids[i].getX(), asteroids[i].getY(), asteroids[i].getWidth(), asteroids[i].getHeight(), asteroids[i].getSize());
			}
		}

		window.clear();
		drawFire();
		window.draw(player.sprite);
		window.draw(text);
		for (int i = 0; i < 20; i++) {
			if (asteroids[i].getStatus()) window.draw(asteroids[i].sprite);
			/*
			if (asteroids[i].getStatus())asteroids[i].sprite.setColor(Color::Red);
			else asteroids[i].sprite.setColor(Color::Green);
			window.draw(asteroids[i].sprite);
			*/
		}
		level =  checkLevelUp(level);
		text.setString(levelText);
		window.display();
	}
	return 0;
}
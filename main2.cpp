#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

// Constants for game configuration
const int resolutionX = 960;
const int resolutionY = 640;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;	  // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Data structure for bullets
struct Bullet
{
	float x, y;
	Sprite sprite; // Sprite is a class for 2D object
};

// Function declarations
void drawPlayer(RenderWindow &window, float &player_x, float &player_y, Sprite &playerSprite);
void handlePlayerMovement(float &player_x, const float speed);
void shootBullet(vector<Bullet> &bullets, const float player_x, const float player_y, const Texture &bulletTexture);
void updateAndDrawBullets(RenderWindow &window, vector<Bullet> &bullets);

int main()
{
	// Initialize the game window
	RenderWindow window(VideoMode(resolutionX, resolutionY), "Buzz Bombers", Style::Close | Style::Titlebar); // Style refers to type of window
	window.setPosition(Vector2i(500, 200));

	// Load background music
	Music bgMusic;
	if (!bgMusic.openFromFile("Music/risk.ogg"))
	{
		cout << "Error: Could not load music file!" << endl;
		return -1;
	}
	bgMusic.setVolume(10);
	bgMusic.setLoop(true);
	bgMusic.play();

	// Initialize player
	float player_x = (gameRows / 2) * boxPixelsX;
	float player_y = (gameColumns - 4) * boxPixelsY;
	Texture playerTexture; // Texture is a class used for storing image
	if (!playerTexture.loadFromFile("Textures/spray.png"))
	{
		cout << "Error: Could not load player texture!" << endl;
		return -1;
	}
	Sprite playerSprite(playerTexture);

	// Initialize bullets
	vector<Bullet> bullets;
	Texture bulletTexture;
	if (!bulletTexture.loadFromFile("Textures/bullet.png"))
	{
		cout << "Error: Could not load bullet texture!" << endl;
		return -1;
	}

	// Ground rectangle
	RectangleShape groundRectangle(Vector2f(resolutionX, boxPixelsY * 2));
	groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);
	groundRectangle.setFillColor(Color::Green);

	// Main game loop
	while (window.isOpen())
	{
		Event e; // Class For inputting Keyborad , Mouse , joystick operations
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
			}
		}

		// Handle player movement
		handlePlayerMovement(player_x, 1.0f);

		// Handle shooting bullets
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			shootBullet(bullets, player_x, player_y, bulletTexture);
		}

		// Clear the window
		window.clear();

		// Draw ground
		window.draw(groundRectangle);

		// Draw player
		drawPlayer(window, player_x, player_y, playerSprite);

		// Update and draw bullets
		updateAndDrawBullets(window, bullets);

		// Display the updated window
		window.display();
	}

	return 0;
}

// Function to draw the player
void drawPlayer(RenderWindow &window, float &player_x, float &player_y, Sprite &playerSprite)
{
	playerSprite.setPosition(player_x, player_y);
	window.draw(playerSprite);
}

// Function to handle player movement
void handlePlayerMovement(float &player_x, const float speed)
{
	if (Keyboard::isKeyPressed(Keyboard::Left) && player_x > 0)
	{
		player_x -= speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right) && player_x < resolutionX - 2 * boxPixelsX)
	{
		player_x += speed;
	}
}

// Function to shoot a bullet
void shootBullet(vector<Bullet> &bullets, const float player_x, const float player_y, const Texture &bulletTexture)
{
	// Limit shooting to prevent spamming
	static Clock shootClock; // class for time
	if (shootClock.getElapsedTime().asMilliseconds() < 200) // for reloading of Bullets
		return;
	shootClock.restart();

	// Create a new bullet
	Bullet newBullet;
	newBullet.x = player_x + (boxPixelsX / 2) - 8; // Center bullet on player
	newBullet.y = player_y;
	newBullet.sprite.setTexture(bulletTexture);
	newBullet.sprite.setScale(3, 3);
	bullets.push_back(newBullet);
}

// Function to update and draw bullets
void updateAndDrawBullets(RenderWindow &window, vector<Bullet> &bullets)
{
	for (auto &bullet : bullets)
	{
		bullet.y -= 1; // Move bullet upward
		bullet.sprite.setPosition(bullet.x, bullet.y);
		window.draw(bullet.sprite);
	}

	// Remove bullets that are off-screen
	bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet &b)
							{ return b.y < -32; }),
				  bullets.end());
}

#include "Camera.h"

Camera::Camera(float x, float y, float z, float speed) : x(x), y(y), z(z), speed(speed) {}

void Camera::Move(float time)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		x += -sin(angleX / 180 * pi) * (speed * time);
		y += tan(angleY / 180 * pi) * (speed * time);
		z += -cos(angleX / 180 * pi) * (speed * time);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		x += sin(angleX / 180 * pi) * (speed * time);
		y += -tan(angleY / 180 * pi) * (speed * time);
		z += cos(angleX / 180 * pi) * (speed * time);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		x += sin((angleX + 90) / 180 * pi) * (speed * time);
		z += cos((angleX + 90) / 180 * pi) * (speed * time);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		x += sin((angleX - 90) / 180 * pi) * (speed * time);
		z += cos((angleX - 90) / 180 * pi) * (speed * time);
	}
}

void Camera::Mouse(sf::RenderWindow& window)
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	sf::Vector2f mousexy = window.mapPixelToCoords(pixelPos);
	
	angleX -= (mousexy.x - window.getSize().x / 2) / 8;
	angleY -= (mousexy.y - window.getSize().y / 2) / 8;
	
	if (angleY < -89.0) angleY = -89.0;
	if (angleY > 89.0) angleY = 89.0;
	
	sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);
}

void Camera::Look()
{
	gluLookAt(x, y, z, x - sin(angleX / 180 * pi), y + tan(angleY / 180 * pi), z - cos(angleX / 180 * pi), 0, 1, 0);
}

void Camera::Look(float x, float y, float z)
{
	gluLookAt(x, y, z, x - sin(angleX / 180 * pi), y + tan(angleY / 180 * pi), z - cos(angleX / 180 * pi), 0, 1, 0);
}

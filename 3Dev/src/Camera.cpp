#include "Camera.hpp"

Camera::Camera(sf::Window* window, rp3d::Vector3 pos, float speed, float fov, float near, float far, Matrices* m) : window(window), pos(pos), speed(speed), fov(fov), near(near), far(far)
{
	if(m) this->m = m;
	aspect = (float)window->getSize().x / (float)window->getSize().y;
	UpdateMatrix();
}

Camera::Camera(sf::Vector2u viewportSize, rp3d::Vector3 pos, float speed, float fov, float near, float far, Matrices* m) : viewportSize(viewportSize), pos(pos), speed(speed), fov(fov), near(near), far(far)
{
    if(m) this->m = m;
	aspect = (float)viewportSize.x / (float)viewportSize.y;
	UpdateMatrix();
}

void Camera::Update(bool force)
{
    sf::Vector2u size = (window ? window->getSize() : viewportSize);
	if((((float)size.x / (float)size.y) != aspect || force))
	{
		aspect = (float)size.x / (float)size.y;
		UpdateMatrix();
	}
}

rp3d::Vector3 Camera::Move(float time, bool onlyOffset)
{
	rp3d::Vector3 offset;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		offset += (orient * rp3d::Vector3(0, 0, -1)) * speed * time;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		offset -= (orient * rp3d::Vector3(0, 0, -1)) * speed * time;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		offset += (orient * rp3d::Vector3(1, 0, 0)) * speed * time;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		offset -= (orient * rp3d::Vector3(1, 0, 0)) * speed * time;

	if(!onlyOffset)
		pos += offset;

	return offset;
}

void Camera::Mouse()
{
    if(window)
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
        sf::Vector2f mousexy = sf::Vector2f((float)pixelPos.x, (float)pixelPos.y);

        orient = rp3d::Quaternion::fromEulerAngles(0, -glm::radians((mousexy.x - window->getSize().x / 2) / 8), 0) * orient;
        auto tmp = orient * rp3d::Quaternion::fromEulerAngles(-glm::radians((mousexy.y - window->getSize().y / 2) / 8), 0, 0);
        if((tmp * rp3d::Vector3(0, 0, -1)).getAbsoluteVector().y < 0.99) orient = tmp;

        sf::Mouse::setPosition(sf::Vector2i(window->getSize().x / 2, window->getSize().y / 2), *window);
    }
}

void Camera::Look()
{
	auto position = GetPosition(true);
	rp3d::Vector3 v = orient * rp3d::Vector3(0, 0, -1), tmpv;
	float tmp;
	if(!alwaysUp) orient.getRotationAngleAxis(tmp, tmpv);
	m->GetView() = glm::lookAt(toglm(position), toglm(position + v), alwaysUp ? glm::vec3(0.0, 1.0, 0.0) : toglm(tmpv));
}

void Camera::Look(const rp3d::Vector3& vec)
{
	auto position = GetPosition(true);
	m->GetView() = glm::lookAt(toglm(position), toglm(vec), glm::vec3(0, 1, 0));
}

void Camera::SetViewportSize(sf::Vector2u size)
{
	viewportSize = size;
	aspect = size.x / (float)size.y;
	UpdateMatrix();
}

void Camera::SetTransform(const rp3d::Transform& tr)
{
	pos = tr.getPosition();
	orient = tr.getOrientation();
}

void Camera::SetPosition(const rp3d::Vector3& vec)
{
	pos = vec;
}

void Camera::SetOrientation(const rp3d::Quaternion& quat)
{
	orient = quat;
}

void Camera::SetSpeed(float speed)
{
	this->speed = speed;
}

void Camera::SetFOV(float fov)
{
	this->fov = fov;
	UpdateMatrix();
}

void Camera::SetNear(float near)
{
	this->near = near;
	UpdateMatrix();
}

void Camera::SetFar(float far)
{
	this->far = far;
	UpdateMatrix();
}

void Camera::AlwaysUp(bool a)
{
	alwaysUp = a;
}

rp3d::Transform Camera::GetTransform()
{
	return rp3d::Transform(pos, orient);
}

rp3d::Vector3 Camera::GetPosition(bool world)
{
	if(!world)
		return pos;
	return (Node::GetFinalTransform(this) * GetTransform()).getPosition();
}

rp3d::Quaternion Camera::GetOrientation()
{
	return orient;
}

float Camera::GetSpeed()
{
	return speed;
}

float Camera::GetFOV()
{
	return fov;
}

float Camera::GetNear()
{
	return near;
}

float Camera::GetFar()
{
	return far;
}

void Camera::UpdateMatrix()
{
	m->GetProjection() = glm::perspective(glm::radians(fov), aspect, near, far);
}

Json::Value Camera::Serialize()
{
    Json::Value data;
    data["position"]["x"] = pos.x;
    data["position"]["y"] = pos.y;
    data["position"]["z"] = pos.z;

    data["orientation"]["x"] = orient.x;
    data["orientation"]["y"] = orient.y;
    data["orientation"]["z"] = orient.z;
    data["orientation"]["w"] = orient.w;

    data["speed"] = speed;
    data["fov"] = fov;
    data["near"] = near;
    data["far"] = far;
    data["alwaysUp"] = alwaysUp;

    return data;
}

void Camera::Deserialize(Json::Value data)
{
    pos.x = data["position"]["x"].asFloat();
    pos.y = data["position"]["y"].asFloat();
    pos.z = data["position"]["z"].asFloat();

    orient.x = data["orientation"]["x"].asFloat();
    orient.y = data["orientation"]["y"].asFloat();
    orient.z = data["orientation"]["z"].asFloat();
    orient.w = data["orientation"]["w"].asFloat();

    speed = data["speed"].asFloat();
    fov = data["fov"].asFloat();
    near = data["near"].asFloat();
    far = data["far"].asFloat();
    alwaysUp = data["alwaysUp"].asBool();
}

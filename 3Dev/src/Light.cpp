#include "Light.hpp"

Light::Light(const rp3d::Vector3& color, const rp3d::Vector3& position, bool castShadows) 
			: castShadows(castShadows), color(color), position(position)
{
	if(castShadows)
		CalcLightSpaceMatrix();
}

void Light::SetColor(const rp3d::Vector3& color)
{
	this->color = color;
}

void Light::SetPosition(const rp3d::Vector3& position)
{
	this->position = position;
}

void Light::SetDirection(const rp3d::Vector3& direction)
{
	this->direction = direction;
}

void Light::SetAttenuation(float constant, float linear, float quadratic)
{
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

void Light::SetCutoff(float cutoff)
{
	this->cutoff = cutoff;
}

void Light::SetOuterCutoff(float outerCutoff)
{
	this->outerCutoff = outerCutoff;
}

void Light::CalcLightSpaceMatrix()
{
	glm::mat4 projection;

	auto tr = Node::GetFinalTransform(this);
	auto pos = (GetTransform() * tr).getPosition();

	if(perspectiveShadows)
		projection = glm::perspective(glm::radians(90.0), 1.0, 0.01, 1000.0);
	else 
		projection = glm::ortho(-40.0, 40.0, -40.0, 40.0, 0.01, 1000.0);
	
	glm::mat4 view = glm::lookAt(toglm(pos), toglm(direction), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = projection * view;
}

void Light::SetIsCastingShadows(bool castShadows)
{
	this->castShadows = castShadows;
}

void Light::SetIsCastingPerspectiveShadows(bool perspectiveShadows)
{
	this->perspectiveShadows = perspectiveShadows;
}

void Light::Update(Shader* shader, int lightnum) 
{
	auto tr = Node::GetFinalTransform(this);
	auto pos = (tr * GetTransform()).getPosition();

	shader->SetUniform3f("lights[" + std::to_string(lightnum) + "].color", color.x, color.y, color.z);
	shader->SetUniform3f("lights[" + std::to_string(lightnum) + "].position", pos.x, pos.y, pos.z);
	shader->SetUniform3f("lights[" + std::to_string(lightnum) + "].direction", direction.x, direction.y, direction.z);
	shader->SetUniform3f("lights[" + std::to_string(lightnum) + "].attenuation", constant, linear, quadratic);
	shader->SetUniform1f("lights[" + std::to_string(lightnum) + "].cutoff", glm::cos(glm::radians(cutoff)));
	shader->SetUniform1f("lights[" + std::to_string(lightnum) + "].outerCutoff", glm::cos(glm::radians(outerCutoff)));
	shader->SetUniform1i("lights[" + std::to_string(lightnum) + "].isactive", 1);
}

bool Light::IsCastingShadows()
{
	return castShadows;
}

bool Light::IsCastingPerspectiveShadows()
{
	return perspectiveShadows;
}

glm::mat4 Light::GetLightSpaceMatrix()
{
	if(castShadows)
		CalcLightSpaceMatrix();
	return lightSpaceMatrix;
}

rp3d::Vector3 Light::GetColor() 
{
	return color;
}

rp3d::Vector3 Light::GetPosition() 
{
	return position;
}

rp3d::Vector3 Light::GetDirection()
{
	return direction;
}

rp3d::Vector3 Light::GetAttenuation()
{
	return rp3d::Vector3(constant, linear, quadratic);
}

rp3d::Transform Light::GetTransform()
{
	return rp3d::Transform(position, rp3d::Quaternion::identity());
}

float Light::GetCutoff()
{
	return cutoff;
}

float Light::GetOuterCutoff()
{
	return outerCutoff;
}

Json::Value Light::Serialize()
{
	Json::Value data;

	data["position"]["x"] = position.x;
	data["position"]["y"] = position.y;
	data["position"]["z"] = position.z;

	data["direction"]["x"] = direction.x;
	data["direction"]["y"] = direction.y;
	data["direction"]["z"] = direction.z;

	data["color"]["r"] = color.x;
	data["color"]["g"] = color.y;
	data["color"]["b"] = color.z;

	data["attenuation"]["constant"] = constant;
	data["attenuation"]["linear"] = linear;
	data["attenuation"]["quadratic"] = quadratic;

	data["cutoff"] = cutoff;
	data["outerCutoff"] = outerCutoff;

	data["castShadows"] = castShadows;
	data["perspectiveShadows"] = perspectiveShadows;

	return data;
}

void Light::Deserialize(Json::Value data)
{
	position.x = data["position"]["x"].asFloat();
	position.y = data["position"]["y"].asFloat();
	position.z = data["position"]["z"].asFloat();

	direction.x = data["direction"]["x"].asFloat();
	direction.y = data["direction"]["y"].asFloat();
	direction.z = data["direction"]["z"].asFloat();

	color.x = data["color"]["r"].asFloat();
	color.y = data["color"]["g"].asFloat();
	color.z = data["color"]["b"].asFloat();

	constant = data["attenuation"]["constant"].asFloat();
	linear = data["attenuation"]["linear"].asFloat();
	quadratic = data["attenuation"]["quadratic"].asFloat();

	cutoff = data["cutoff"].asFloat();
	outerCutoff = data["outerCutoff"].asFloat();

	castShadows = data["castShadows"].asBool();
	perspectiveShadows = data["perspectiveShadows"].asBool();

	if(castShadows)
		CalcLightSpaceMatrix();
}

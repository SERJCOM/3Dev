#include "Shape.h"

Shape::Shape(float w, float h, float d, float x, float y, float z) : size(w, h, d), position(x, y, z) {}

void Shape::Draw(GLuint texture, float x, float y, float z, float w, float h, float d)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glNormal3f(0.0, 0.0, 1.0); glVertex3f(-w, -h, -d);
	glTexCoord2f(1, 0); glVertex3f(w, -h, -d);
	glTexCoord2f(1, 1); glVertex3f(w, h, -d);
	glTexCoord2f(0, 1); glVertex3f(-w, h, -d);

	glTexCoord2f(0, 0); glNormal3f(-1.0, 0.0, 0.0); glVertex3f(w, -h, d);
	glTexCoord2f(1, 0); glVertex3f(-w, -h, d);
	glTexCoord2f(1, 1); glVertex3f(-w, h, d);
	glTexCoord2f(0, 1); glVertex3f(w, h, d);

	glTexCoord2f(0, 0); glNormal3f(0.0, 0.0, -1.0); glVertex3f(-w, -h, d);
	glTexCoord2f(1, 0); glVertex3f(-w, -h, -d);
	glTexCoord2f(1, 1); glVertex3f(-w, h, -d);
	glTexCoord2f(0, 1); glVertex3f(-w, h, d);

	glTexCoord2f(0, 0); glNormal3f(1.0, 0.0, 0.0); glVertex3f(w, -h, -d);
	glTexCoord2f(1, 0); glVertex3f(w, -h, d);
	glTexCoord2f(1, 1); glVertex3f(w, h, d);
	glTexCoord2f(0, 1); glVertex3f(w, h, -d);

	glTexCoord2f(0, 0); glNormal3f(0.0, 1.0, 0.0); glVertex3f(-w, -h, d);
	glTexCoord2f(1, 0); glVertex3f(w, -h, d);
	glTexCoord2f(1, 1); glVertex3f(w, -h, -d);
	glTexCoord2f(0, 1); glVertex3f(-w, -h, -d);

	glTexCoord2f(0, 0); glNormal3f(0.0, -1.0, 0.0); glVertex3f(-w, h, -d);
	glTexCoord2f(1, 0); glVertex3f(w, h, -d);
	glTexCoord2f(1, 1); glVertex3f(w, h, d);
	glTexCoord2f(0, 1); glVertex3f(-w, h, d);

	glEnd();
	glPopMatrix();
}

void Shape::Draw(GLuint texture[6], float x, float y, float z, float w, float h, float d)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glNormal3f(0.0, 0.0, 1.0); glVertex3f(-w, -h, -d);
	glTexCoord2f(1, 0); glVertex3f(w, -h, -d);
	glTexCoord2f(1, 1); glVertex3f(w, h, -d);
	glTexCoord2f(0, 1); glVertex3f(-w, h, -d);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glNormal3f(-1.0, 0.0, 0.0); glVertex3f(w, -h, d);
	glTexCoord2f(1, 0); glVertex3f(-w, -h, d);
	glTexCoord2f(1, 1); glVertex3f(-w, h, d);
	glTexCoord2f(0, 1); glVertex3f(w, h, d);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glNormal3f(0.0, 0.0, -1.0); glVertex3f(-w, -h, d);
	glTexCoord2f(1, 0); glVertex3f(-w, -h, -d);
	glTexCoord2f(1, 1); glVertex3f(-w, h, -d);
	glTexCoord2f(0, 1); glVertex3f(-w, h, d);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glNormal3f(1.0, 0.0, 0.0); glVertex3f(w, -h, -d);
	glTexCoord2f(1, 0); glVertex3f(w, -h, d);
	glTexCoord2f(1, 1); glVertex3f(w, h, d);
	glTexCoord2f(0, 1); glVertex3f(w, h, -d);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glNormal3f(0.0, 1.0, 0.0); glVertex3f(-w, -h, d);
	glTexCoord2f(1, 0); glVertex3f(w, -h, d);
	glTexCoord2f(1, 1); glVertex3f(w, -h, -d);
	glTexCoord2f(0, 1); glVertex3f(-w, -h, -d);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glNormal3f(0.0, -1.0, 0.0); glVertex3f(-w, h, -d);
	glTexCoord2f(1, 0); glVertex3f(w, h, -d);
	glTexCoord2f(1, 1); glVertex3f(w, h, d);
	glTexCoord2f(0, 1); glVertex3f(-w, h, d);
	glEnd();
	glPopMatrix();
}

void Shape::Draw(GLuint texture) 
{
	Draw(texture, position.x, position.y, position.z, size.x, size.y, size.z);
}

void Shape::Draw(GLuint texture[6])
{
	Draw(texture, position.x, position.y, position.z, size.x, size.y, size.z);
}

void Shape::DrawPlane(GLuint texture) {
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0, 0); glVertex3f(-size.x, 0, -size.z);
	glTexCoord2f(1, 0); glVertex3f(size.x, 0, -size.z);
	glTexCoord2f(1, 1); glVertex3f(size.x, 0, size.z);
	glTexCoord2f(0, 1); glVertex3f(-size.x, 0, size.z);

	glEnd();
	glPopMatrix();
}

void Shape::SetPosition(float x, float y, float z)
{
	position = sf::Vector3f(x, y, z);
}

void Shape::SetSize(float w, float h, float d)
{
	size = sf::Vector3f(w, h, d);
}

sf::Vector3f Shape::GetPosition() 
{
	return position;
}

sf::Vector3f Shape::GetSize() 
{
	return size;
}

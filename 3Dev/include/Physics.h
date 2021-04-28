#include <3Dev.h>

float collision(float x, float y, float z, Model& m, float p = 1)
{
	for (int j = 0; j < m.numVerts * 3; j += 3) {
		if(abs(x - ((m.GetPosition().x + m.vertexArray[j]) * m.GetSize().x)) <= p * m.GetSize().x && abs(z - ((m.GetPosition().z + m.vertexArray[j + 2] * m.GetSize().z))) <= p * m.GetSize().z)
		{
			if (y <= (m.GetPosition().y + m.vertexArray[j + 1]) * m.GetSize().y && y >= (m.GetPosition().y - m.vertexArray[j + 1]) / m.GetSize().y) {
				return ((m.GetPosition().y + m.vertexArray[j + 1]) * m.GetSize().y) - y;
			}
		}
	}
	return 0;
}

bool collision(Model& m, float x, float y, float z, float w = 0, float h = 0, float d = 0)
{
	for (int j = 0; j < m.numVerts * 3; j += 3) {
		if((m.GetPosition().x + m.vertexArray[j] <= x + w && m.GetPosition().x + m.vertexArray[j] >= x - w) && (m.GetPosition().z + m.vertexArray[j + 2] <= z + d && m.GetPosition().z + m.vertexArray[j + 2] >= z - d))
		{
			if (m.GetPosition().y + m.vertexArray[j + 1] <= y + h && m.GetPosition().y + m.vertexArray[j + 1] >= y - h) {
				return true;
			}
		}
	}
	return false;
}

bool collision(float x, float y, float z, float xx, float yy, float zz, float w = 0, float h = 0, float d = 0)
{
	if((x <= xx + w && x >= xx - w) && (z <= zz + d && z >= zz - d))
	{
		if (y <= yy + h && y >= yy - h) {
			return true;
		}
	}
	return false;
}

float collision(float x, float y, float z, Shape s)
{
	if((x <= s.GetPosition().x + s.GetSize().x && x >= s.GetPosition().x - s.GetSize().x) && (z <= s.GetPosition().z + s.GetSize().z && z >= s.GetPosition().z - s.GetSize().z))
	{
		if (y <= s.GetPosition().y + s.GetSize().y && y >= s.GetPosition().y - s.GetSize().y)
		{
			return (s.GetPosition().y + s.GetSize().y) - y;
		}
	}
	return 0;
}

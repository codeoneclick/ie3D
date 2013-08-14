/*
 Copyright (C) 2011-2012 Thijs Ferket (RdR)

 This software is provided 'as-is', without any express or implied
 warranty.  In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 */

#include "CProgressiveVertex.h"

CProgressiveVertex::CProgressiveVertex(ui32 _id, const glm::vec3& _position, const glm::vec2& _texcoord, const glm::vec3& _normal) :
m_id(_id),
m_position(_position),
m_texcoord(_texcoord),
m_normal(_normal),
m_quadric(glm::mat4x4(-1)),
m_isActive(true)
{
}

CProgressiveVertex::~CProgressiveVertex(void)
{
	m_faces.clear();
}

void CProgressiveVertex::AddFace(std::shared_ptr<CProgressiveTriangle> _triangle)
{
	if (_triangle && _triangle->isActive())
    {
		bool add = true;
		for(ui32 i = 0; i < m_faces.size(); ++i)
        {
			std::shared_ptr<CProgressiveTriangle> triangle = m_faces[i];
			if (_triangle == triangle)
            {
				add = false;
				break;
			}
		}
		if (add)
        {
			m_faces.push_back(_triangle);
		}
	}
}

void CProgressiveVertex::RemoveFace(std::shared_ptr<CProgressiveTriangle> _triangle)
{
    std::vector<std::shared_ptr<CProgressiveTriangle> >::iterator iterator = m_faces.begin();
	for(; iterator != m_faces.end(); ++iterator)
    {
		if (_triangle == (*iterator))
        {
			m_faces.erase(iterator);
			break;
		}
	}
}

void CProgressiveVertex::CalculateQuadric(bool _useTriangleArea)
{
	m_quadric = glm::mat4x4(0.0f);

	for(ui32 i = 0; i < m_faces.size(); ++i)
    {
		std::shared_ptr<CProgressiveTriangle> triangle = m_faces[i];

		if(triangle->isActive())
        {
			f32 triangleArea = 1;
			if (_useTriangleArea)
            {
				triangleArea = triangle->getArea();
				quadricTriangleArea += triangleArea;
			}

			const glm::vec3 normal = triangle->Get_Normal();
			const f32 a = normal.x;
			const f32 b = normal.y;
			const f32 c = normal.z;
			const f32 d = glm::dot(normal, triangle->Get_Vertex1()->Get_Position());

			quadric[0][0] += triangleArea * a * a;
			quadric[0][1] += triangleArea * a * b;
			quadric[0][2] += triangleArea * a * c;
			quadric[0][3] += triangleArea * a * d;

			quadric[1][0] += triangleArea * b * a;
			quadric[1][1] += triangleArea * b * b;
			quadric[1][2] += triangleArea * b * c;
			quadric[1][3] += triangleArea * b * d;

			quadric[2][0] += triangleArea * c * a;
			quadric[2][1] += triangleArea * c * b;
			quadric[2][2] += triangleArea * c * c;
			quadric[2][3] += triangleArea * c * d;

			quadric[3][0] += triangleArea * d * a;
			quadric[3][1] += triangleArea * d * b;
			quadric[3][2] += triangleArea * d * c;
			quadric[3][3] += triangleArea * d * d;
		}
	}
}

bool CProgressiveVertex::IsOrphan(void) const
{
	return (m_faces.size() == 0);
}

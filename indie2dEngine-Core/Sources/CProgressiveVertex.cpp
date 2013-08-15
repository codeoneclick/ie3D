
#include "CProgressiveVertex.h"
#include "CProgressiveTriangle.h"

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
	if (_triangle && _triangle->IsActive())
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

		if(triangle->IsActive())
        {
			f32 triangleArea = 1;
			if (_useTriangleArea)
            {
				triangleArea = triangle->Get_Area();
				m_quadricTriangleArea += triangleArea;
			}

			const glm::vec3 normal = triangle->Get_Normal();
			const f32 a = normal.x;
			const f32 b = normal.y;
			const f32 c = normal.z;
			const f32 d = glm::dot(normal, triangle->Get_Vertex01()->Get_Position());

			m_quadric[0][0] += triangleArea * a * a;
			m_quadric[0][1] += triangleArea * a * b;
			m_quadric[0][2] += triangleArea * a * c;
			m_quadric[0][3] += triangleArea * a * d;

			m_quadric[1][0] += triangleArea * b * a;
			m_quadric[1][1] += triangleArea * b * b;
			m_quadric[1][2] += triangleArea * b * c;
			m_quadric[1][3] += triangleArea * b * d;

			m_quadric[2][0] += triangleArea * c * a;
			m_quadric[2][1] += triangleArea * c * b;
			m_quadric[2][2] += triangleArea * c * c;
			m_quadric[2][3] += triangleArea * c * d;

			m_quadric[3][0] += triangleArea * d * a;
			m_quadric[3][1] += triangleArea * d * b;
			m_quadric[3][2] += triangleArea * d * c;
			m_quadric[3][3] += triangleArea * d * d;
		}
	}
}

bool CProgressiveVertex::IsOrphan(void) const
{
	return (m_faces.size() == 0);
}

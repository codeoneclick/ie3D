
#ifndef CProgressiveTriangle_h
#define CProgressiveTriangle_h


#include "CProgressiveVertex.h"
#include "CProgressiveEdge.h"

class CProgressiveTriangle
{
private:
    
	CProgressiveVertex* m_vertexes[3];
	bool m_active;
    
protected:

public:

	CProgressiveTriangle(CProgressiveVertex* _vertex_01, CProgressiveVertex* _vertex_02, CProgressiveVertex* _vertex_03);
	virtual ~CProgressiveTriangle(void);
    
    inline CProgressiveVertex* Get_Vertex01(void) const
    {
        return m_vertexes[0];
    };
    
    inline CProgressiveVertex* Get_Vertex02(void) const
    {
        return m_vertexes[1];
    }
    
    inline CProgressiveVertex* Get_Vertex03(void) const
    {
        return m_vertexes[2];
    };
    
    inline CProgressiveVertex* Get_Vertex(ui32 _index) const
    {
        if (_index > 3)
        {
            _index = 0;
        }
        return m_vertexes[_index];
    };
    
    inline bool IsActive(void) const
    {
        return m_active;
    };
    
    inline void Set_Active(bool _active)
    {
        m_active = _active;
    };
    
    inline glm::vec3 Get_Normal(void) const
    {
        glm::vec3 edge_01 = m_vertexes[1]->Get_Position() - m_vertexes[0]->Get_Position();
        glm::vec3 edge_02 = m_vertexes[2]->Get_Position() - m_vertexes[0]->Get_Position();
        return glm::cross(edge_01, edge_02);
    };
    
    inline f32 Get_Area(void) const
    {
        glm::vec3 edge_01 = m_vertexes[1]->Get_Position() - m_vertexes[0]->Get_Position();
        glm::vec3 edge_02 = m_vertexes[2]->Get_Position() - m_vertexes[0]->Get_Position();
        glm::vec3 normal = glm::cross(edge_01, edge_02);
        return glm::length(normal) * 0.5f;
    };
    
    inline bool HasVertex(CProgressiveVertex* _vertex) const
    {
        return (_vertex == m_vertexes[0] || _vertex == m_vertexes[1] || _vertex == m_vertexes[2]);
    };

	void ReplaceVertex(CProgressiveVertex* _from, CProgressiveVertex* _to);
};

#endif


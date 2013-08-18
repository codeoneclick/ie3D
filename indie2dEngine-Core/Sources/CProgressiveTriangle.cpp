
#include "CProgressiveTriangle.h"

CProgressiveTriangle::CProgressiveTriangle(CProgressiveVertex* _vertex_01, CProgressiveVertex* _vertex_02, CProgressiveVertex* _vertex_03) 
{
	m_vertexes[0] = _vertex_01;
	m_vertexes[1] = _vertex_02;
	m_vertexes[2] = _vertex_03;

	m_active = true;

	for(ui32 i = 0; i < 3; ++i)
    {
		m_vertexes[i]->AddFace(shared_from_this());
	}
}

CProgressiveTriangle::~CProgressiveTriangle(void)
{
	m_vertexes[0]->RemoveFace(shared_from_this());
	m_vertexes[1]->RemoveFace(shared_from_this());
	m_vertexes[2]->RemoveFace(shared_from_this());
}

void CProgressiveTriangle::ReplaceVertex(CProgressiveVertex* _from, CProgressiveVertex* _to)
{
	_from->RemoveFace(shared_from_this());

	for (ui32 i = 0; i < 3; ++i)
    {
		if (m_vertexes[i] == _from)
        {
			m_vertexes[i] = _to;
			m_vertexes[i]->AddFace(shared_from_this());
			break;
		}
	}
}
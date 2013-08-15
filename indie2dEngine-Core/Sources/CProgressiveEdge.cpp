
#include "CProgressiveEdge.h"
#include "CProgressiveTriangle.h"

CProgressiveEdge::CProgressiveEdge(CProgressiveVertex* _from, CProgressiveVertex* _to) :
m_from(_from),
m_to(_to),
m_cost(0)
{
    CProgressiveEdge::Update();
}

CProgressiveEdge::~CProgressiveEdge(void)
{
	m_faces.clear();
}

void CProgressiveEdge::Update(void)
{
	m_faces.clear();

	const std::vector<std::shared_ptr<CProgressiveTriangle>> facesFrom = m_from->Get_Faces();
	for (ui32 i = 0; i < facesFrom.size(); ++i)
    {
		std::shared_ptr<CProgressiveTriangle> triangle_01 = facesFrom[i];
		if (triangle_01 && triangle_01->IsActive())
        {
			const std::vector<std::shared_ptr<CProgressiveTriangle>> facesTo = m_to->Get_Faces();
			for (ui32 j = 0; j < facesTo.size(); ++j)
            {
				std::shared_ptr<CProgressiveTriangle> triangle_02 = facesTo[j];
				if (triangle_01 == triangle_02)
                {
					m_faces.push_back(triangle_01);
				}
			}
		}
	}
}

void CProgressiveEdge::AddFace(std::shared_ptr<CProgressiveTriangle> _triangle)
{
	if (_triangle && _triangle->IsActive())
    {
		bool add = true;
		for (ui32 i = 0; i < m_faces.size(); ++i)
        {
			std::shared_ptr<CProgressiveTriangle> triangle = m_faces[i];
			if (triangle == _triangle)
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

void CProgressiveEdge::RemoveFace(std::shared_ptr<CProgressiveTriangle> _triangle)
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

bool CProgressiveEdge::IsBorder(void) const
{
	return (m_faces.size() == 1);
}

bool CProgressiveEdge::HasVertex(CProgressiveVertex* _vertex) const
{
	return (_vertex == m_from || _vertex == m_to);
}

void CProgressiveEdge::ReplaceVertex(CProgressiveVertex* _from, CProgressiveVertex* _to)
{
	if (m_from == _from)
    {
		m_from = _to;
	}
    else if (m_to == _from)
    {
		m_to = _to;
	}
    CProgressiveEdge::Update();
}

bool CProgressiveEdge::IsValid(void) const
{
	if(!m_from || !m_to)
    {
		return false;
	}
	if(!m_from->IsActive() || !m_to->IsActive())
    {
		return false;
	}
	if(m_from == m_to)
    {
		return false;
	}
	if(m_faces.size() == 0)
    {
		return false;
	}
	return true;
}

std::string CProgressiveEdge::Get_IdString(void)
{
	std::string string = "";
    
	if (m_from->Get_Id() < m_to->Get_Id())
    {
        std::stringstream stringstream_01;
        stringstream_01 << m_from->Get_Id();
		string += stringstream_01.str();
		string += ";";
        
        std::stringstream stringstream_02;
        stringstream_02 << m_to->Get_Id();
		string += stringstream_02.str();
	}
    else
    {
		std::stringstream stringstream_01;
        stringstream_01 << m_to->Get_Id();
		string += stringstream_01.str();
		string += ";";
        
        std::stringstream stringstream_02;
        stringstream_02 << m_from->Get_Id();
		string += stringstream_02.str();
	}
	return string;
}

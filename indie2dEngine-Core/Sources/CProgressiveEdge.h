/*
 * Edge.h
 *
 * Tank @ War Project
 * October 2011
 */

#ifndef CProgressiveEdge_h
#define CProgressiveEdge_h

#include "CProgressiveVertex.h"

class CProgressiveTriangle;

class CProgressiveEdge
{
private:
    
    CProgressiveVertex* m_from;
	CProgressiveVertex* m_to;
    f32 m_cost;
    std::vector<std::shared_ptr<CProgressiveTriangle>> m_faces;

protected:
    
public:

	CProgressiveEdge(CProgressiveVertex* _from, CProgressiveVertex* _to);
	virtual ~CProgressiveEdge(void);

	inline CProgressiveVertex* Get_VertexFrom(void) const
    {
        return m_from;
    };
    
    inline CProgressiveVertex* Get_VertexTo(void) const
    {
        return m_to;
    };

	inline f32 Get_Cost(void) const
    {
        return m_cost;
    };
    
	inline void Set_Cost(f32 _cost)
    {
        m_cost = _cost;
    };

	bool IsBorder(void) const;


    inline std::vector<std::shared_ptr<CProgressiveTriangle> >& Get_Faces(void)
    {
        return m_faces;
    };

	void Update(void);

	void AddFace(std::shared_ptr<CProgressiveTriangle> _triangle);
	void RemoveFace(std::shared_ptr<CProgressiveTriangle> _triangle);

	bool HasVertex(CProgressiveVertex* _vertex) const;
	void ReplaceVertex(CProgressiveVertex* _from, CProgressiveVertex* _to);
	bool IsValid() const;

    std::string Get_IdString(void);

	bool operator<(const CProgressiveEdge& _other) const
    {
		return (Get_Cost() < _other.Get_Cost());
	};

	bool operator==(const CProgressiveEdge& _other) const
    {
		return (Get_Cost() == _other.Get_Cost());
	};
};

#endif

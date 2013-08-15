
#ifndef ProgressiveMesh_h
#define ProgressiveMesh_h

#include "CProgressiveVertex.h"
#include "CProgressiveEdge.h"
#include "CProgressiveTriangle.h"

class CMesh;

enum E_SIMPLIFICATION_ALGORITHM
{
    E_SIMPLIFICATION_ALGORITHM_RANDOM,
    E_SIMPLIFICATION_ALGORITHM_SHORTEST,
    E_SIMPLIFICATION_ALGORITHM_MELAX,
    E_SIMPLIFICATION_ALGORITHM_QUADRIC,
    E_SIMPLIFICATION_ALGORITHM_QUADRICTRI
};


class ProgressiveMesh
{
private:
    
    std::shared_ptr<CMesh> m_mesh;
    E_SIMPLIFICATION_ALGORITHM m_algorithm;
    std::vector<CProgressiveVertex*> m_vertexes;
    std::vector<std::shared_ptr<CProgressiveTriangle>> m_faces;
    std::vector<std::shared_ptr<CProgressiveEdge>> m_edges;
    
    void _CalculateQuadric(bool _useTriangleArea = true);
	void _CreateEdges(void);
	void _CalculateEdgeCost(void);
	void _CalculateEdgeCost(std::shared_ptr<CProgressiveEdge> _edge);
	void _CalculateRandomEdgeCost(std::shared_ptr<CProgressiveEdge> _edge);
	void _CalculateShortEdgeCost(std::shared_ptr<CProgressiveEdge> _edge);
	void _CalculateMelaxEdgeCost(std::shared_ptr<CProgressiveEdge> _edge);
	void _CalculateQuadricEdgeCost(std::shared_ptr<CProgressiveEdge> _edge);
	void _ApplyBorderPenalties(std::shared_ptr<CProgressiveEdge> _edge);
	f32 _CalcQuadricError(const glm::mat4x4& _quadric, CProgressiveVertex* _vertex, f32 _triangleArea);
	void _Contract(std::shared_ptr<CProgressiveEdge> _edge);
    
protected:
    

public:

	ProgressiveMesh(std::shared_ptr<CMesh> _mesh, E_SIMPLIFICATION_ALGORITHM _algorithm = E_SIMPLIFICATION_ALGORITHM_QUADRICTRI);
	virtual ~ProgressiveMesh(void);

	inline const std::vector<std::shared_ptr<CProgressiveTriangle>>& Get_Triangles(void) const
    {
        return m_faces;
    };

	inline const std::vector<CProgressiveVertex*>& Get_Vertices(void) const
    {
        return m_vertexes;
    };
    
    inline E_SIMPLIFICATION_ALGORITHM Get_Algorithm(void) const
    {
        return m_algorithm;
    };
    

	std::shared_ptr<CMesh> Get_CurrentMesh(void);

	void Contract(ui32 _numberOfEdges = 1);
	void ContractTillTriangleCount(ui32 _numberOfTriangles);

};

#endif 

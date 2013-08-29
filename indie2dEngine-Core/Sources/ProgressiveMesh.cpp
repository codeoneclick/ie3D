
#include "ProgressiveMesh.h"
#include "CMesh.h"

/*
static const ui32 BOUNDARY_WEIGHT = 1000;

ProgressiveMesh::ProgressiveMesh(std::shared_ptr<CMesh> _mesh, E_SIMPLIFICATION_ALGORITHM _algorithm)
{
	m_algorithm = _algorithm;
	m_mesh = _mesh;

    std::shared_ptr<CVertexBuffer> vertexBuffer = m_mesh->Get_VertexBuffer();
    SVertex* vertexData = vertexBuffer->Lock();
    
    for(ui32 i = 0; i < vertexBuffer->Get_Size(); ++i)
    {
        m_vertexes.push_back(new CProgressiveVertex(i, vertexData[i].m_position, vertexData[i].m_texcoord, CVertexBuffer::UncompressU8Vec4(vertexData[i].m_normal)));
    }
    
    std::shared_ptr<CIndexBuffer> indexBuffer = m_mesh->Get_IndexBuffer();
	ui16* indexes = indexBuffer->Lock();
	for (ui32 i = 0; i < indexBuffer->Get_Size(); i += 3)
    {
		std::shared_ptr<CProgressiveTriangle> triangle = std::make_shared<CProgressiveTriangle>(m_vertexes[indexes[i]], m_vertexes[indexes[i + 1]], m_vertexes[indexes[i + 2]]);
        m_faces.push_back(triangle);
	}

	if (_algorithm == E_SIMPLIFICATION_ALGORITHM_QUADRICTRI)
    {
		_CalculateQuadric(true);
	}
    else if (_algorithm == E_SIMPLIFICATION_ALGORITHM_QUADRIC)
    {
		_CalculateQuadric(false);
	}

	_CreateEdges();
	_CalculateEdgeCost();
}

ProgressiveMesh::~ProgressiveMesh(void)
{
	m_edges.clear();
	m_faces.clear();

	for (ui32 i = 0; i < m_vertexes.size(); ++i)
    {
		CProgressiveVertex* vertex = m_vertexes[i];
		delete vertex;
	}
	m_vertexes.clear();
}

std::shared_ptr<CMesh> ProgressiveMesh::Get_CurrentMesh(void)
{
    std::vector<SVertex> vertexes;
	ui32 index = 0;
    for(ui32 i = 0; i < m_vertexes.size(); ++i)
    {
        CProgressiveVertex* vertex = m_vertexes[i];
        if (vertex->IsOrphan())
        {
            vertex->Set_Active(false);
        }
        else if (vertex && vertex->IsActive())
        {
            vertex->Set_Id(index);
            
            SVertex result;
            result.m_position = vertex->Get_Position();
            result.m_normal = CVertexBuffer::CompressVec3(vertex->Get_Normal());
            result.m_texcoord = vertex->Get_Texcoord();
            vertexes.push_back(result);
            index++;
        }
		
	}

    std::shared_ptr<CVertexBuffer> vertexBuffer = std::make_shared<CVertexBuffer>(vertexes.size(), GL_STATIC_DRAW);
    SVertex* vertexData = vertexBuffer->Lock();
    for(ui32 i = 0; i < vertexes.size(); ++i)
    {
        vertexData[i] = vertexes[i];
    }
    vertexBuffer->Unlock();
    
    std::vector<ui16> indexes;

	for (ui32 i = 0; i < m_faces.size(); ++i)
    {
		const std::shared_ptr<CProgressiveTriangle> triangle = m_faces[i];
		if (triangle && triangle->IsActive())
        {
			indexes.push_back(triangle->Get_Vertex(0)->Get_Id());
			indexes.push_back(triangle->Get_Vertex(1)->Get_Id());
			indexes.push_back(triangle->Get_Vertex(2)->Get_Id());
		}
	}
    
    std::shared_ptr<CIndexBuffer> indexBuffer = std::make_shared<CIndexBuffer>(indexes.size(), GL_STATIC_DRAW);
    ui16* indexData = indexBuffer->Lock();
    
    for(ui32 i = 0; i < indexes.size(); ++i)
    {
        indexData[i] = indexes[i];
    }
    indexBuffer->Unlock();
	return nullptr;
}

void ProgressiveMesh::_CalculateQuadric(bool _useTriangleArea)
{
	for(ui32 i = 0; i < m_vertexes.size(); ++i)
    {
		CProgressiveVertex* vertex = m_vertexes[i];
		vertex->CalculateQuadric(_useTriangleArea);
	}
}

void ProgressiveMesh::_CreateEdges(void)
{
    std::map<std::string, std::shared_ptr<CProgressiveEdge>> uniqueEdges;
	for (ui32 i = 0; i < m_faces.size(); ++i)
    {
		const std::shared_ptr<CProgressiveTriangle> triangle = m_faces[i];

		for (ui32 j = 0; j < 3; ++j)
        {
			CProgressiveVertex* vertexFrom = 0;
			CProgressiveVertex* vertexTo = 0;

			if (j == 0)
            {
				vertexFrom = triangle->Get_Vertex(0);
				vertexTo = triangle->Get_Vertex(1);
			}
            else if (j == 1)
            {
				vertexFrom = triangle->Get_Vertex(1);
				vertexTo = triangle->Get_Vertex(2);
			}
            else
            {
				vertexFrom = triangle->Get_Vertex(2);
				vertexTo = triangle->Get_Vertex(0);
			}
            
            std::string string = "";
            
            if (vertexFrom->Get_Id() < vertexTo->Get_Id())
            {
                std::stringstream stringstream_01;
                stringstream_01 << vertexFrom->Get_Id();
                string += stringstream_01.str();
                string += ";";
                
                std::stringstream stringstream_02;
                stringstream_02 << vertexTo->Get_Id();
                string += stringstream_02.str();
            }
            else
            {
                std::stringstream stringstream_01;
                stringstream_01 << vertexTo->Get_Id();
                string += stringstream_01.str();
                string += ";";
                
                std::stringstream stringstream_02;
                stringstream_02 << vertexFrom->Get_Id();
                string += stringstream_02.str();
            }
		
            std::map<std::string, std::shared_ptr<CProgressiveEdge>>::iterator iterator = uniqueEdges.find(string);
			if (iterator != uniqueEdges.end())
            {
				std::shared_ptr<CProgressiveEdge> edge = std::make_shared<CProgressiveEdge>(vertexFrom, vertexTo);
				uniqueEdges.insert(std::make_pair(string, edge));
				m_edges.push_back(edge);
			}
		}
	}
}

void ProgressiveMesh::_CalculateEdgeCost(void)
{
	for (ui32 i = 0; i < m_edges.size(); ++i)
    {
		std::shared_ptr<CProgressiveEdge> edge = m_edges[i];
		_CalculateEdgeCost(edge);
	}
}

void ProgressiveMesh::_CalculateEdgeCost(std::shared_ptr<CProgressiveEdge> _edge)
{
	switch (m_algorithm)
    {
	case E_SIMPLIFICATION_ALGORITHM_RANDOM:
		_CalculateRandomEdgeCost(_edge);
		break;
	case E_SIMPLIFICATION_ALGORITHM_SHORTEST:
		_CalculateShortEdgeCost(_edge);
		break;
	case E_SIMPLIFICATION_ALGORITHM_MELAX:
		_CalculateMelaxEdgeCost(_edge);
		break;
	case E_SIMPLIFICATION_ALGORITHM_QUADRIC:
	case E_SIMPLIFICATION_ALGORITHM_QUADRICTRI:
		_CalculateQuadricEdgeCost(_edge);
		break;
	default:
		break;
	};
}

void ProgressiveMesh::_CalculateRandomEdgeCost(std::shared_ptr<CProgressiveEdge> _edge)
{
	const f32 cost = f32(rand() % m_edges.size());
	_edge->Set_Cost(cost);
}

void ProgressiveMesh::_CalculateShortEdgeCost(std::shared_ptr<CProgressiveEdge> _edge)
{
	const f32 edgeLength = glm::length(_edge->Get_VertexTo()->Get_Position() - _edge->Get_VertexFrom()->Get_Position());
	_edge->Set_Cost(edgeLength);
}

void ProgressiveMesh::_CalculateMelaxEdgeCost(std::shared_ptr<CProgressiveEdge> _edge)
{
	const f32 edgeLength = glm::length(_edge->Get_VertexTo()->Get_Position() - _edge->Get_VertexFrom()->Get_Position());
	f32 curvature = 0;

	const std::vector<std::shared_ptr<CProgressiveTriangle>> facesEdge = _edge->Get_Faces();
	const std::vector<std::shared_ptr<CProgressiveTriangle>> facesFrom = _edge->Get_VertexFrom()->Get_Faces();

	for(ui32 i = 0; i < facesFrom.size(); ++i)
    {
		const std::shared_ptr<CProgressiveTriangle> triangle_01 = facesFrom[i];
		f32 minCurve = 1;

		for (ui32 j = 0; j < facesEdge.size(); ++j)
        {
			const std::shared_ptr<CProgressiveTriangle> triangle_02 = facesEdge[j];
			const f32 dotProduct = glm::dot(triangle_01->Get_Normal(), triangle_02->Get_Normal());
			minCurve =  glm::min(minCurve, (1 - dotProduct) / 2.0f);
		}
		curvature = glm::max(curvature, minCurve);
	}

	const f32 cost = edgeLength * curvature;
	_edge->Set_Cost(cost);
}

void ProgressiveMesh::_CalculateQuadricEdgeCost(std::shared_ptr<CProgressiveEdge> _edge)
{
	if (_edge->IsBorder())
    {
		_ApplyBorderPenalties(_edge);
	}

	const glm::mat4x4 quadric_01 = _edge->Get_VertexFrom()->Get_Quadric();
	const glm::mat4x4 quadric_02 = _edge->Get_VertexTo()->Get_Quadric();
	const glm::mat4x4 quadricSum = quadric_01 + quadric_02;

	f32 triangleArea = 0;

	CProgressiveVertex* vertex = _edge->Get_VertexTo();

	const f32 cost = _CalcQuadricError(quadricSum,vertex, triangleArea);
	_edge->Set_Cost(cost);
}

void ProgressiveMesh::_ApplyBorderPenalties(std::shared_ptr<CProgressiveEdge> _edge)
{
    glm::vec3 position_01 = _edge->Get_VertexFrom()->Get_Position();
	glm::vec3 position_02 = _edge->Get_VertexTo()->Get_Position();
	glm::vec3 distance = position_01 - position_02;
	glm::vec3 normal = _edge->Get_Faces()[0]->Get_Normal();
	glm::vec3 abc = glm::cross(distance, normal);
    abc = glm::normalize(abc);

	f32& a = abc.x;
	f32& b = abc.y;
	f32& c = abc.z;

	f32 d = -(glm::dot(abc, position_01));

    glm::mat4x4 quadricConstraint;
	quadricConstraint[0][0] = BOUNDARY_WEIGHT * a * a;
	quadricConstraint[0][1] = BOUNDARY_WEIGHT * a * b;
	quadricConstraint[0][2] = BOUNDARY_WEIGHT * a * c;
	quadricConstraint[0][3] = BOUNDARY_WEIGHT * a * d;

	quadricConstraint[1][0] = BOUNDARY_WEIGHT * b * a;
	quadricConstraint[1][1] = BOUNDARY_WEIGHT * b * b;
	quadricConstraint[1][2] = BOUNDARY_WEIGHT * b * c;
	quadricConstraint[1][3] = BOUNDARY_WEIGHT * b * d;

	quadricConstraint[2][0] = BOUNDARY_WEIGHT * c * a;
	quadricConstraint[2][1] = BOUNDARY_WEIGHT * c * b;
	quadricConstraint[2][2] = BOUNDARY_WEIGHT * c * c;
	quadricConstraint[2][3] = BOUNDARY_WEIGHT * c * d;

	quadricConstraint[3][0] = BOUNDARY_WEIGHT * d * a;
	quadricConstraint[3][1] = BOUNDARY_WEIGHT * d * b;
	quadricConstraint[3][2] = BOUNDARY_WEIGHT * d * c;
	quadricConstraint[3][3] = BOUNDARY_WEIGHT * d * d;

	glm::mat4x4 quadric_01 = _edge->Get_VertexFrom()->Get_Quadric();
	glm::mat4x4 quadric_02 = _edge->Get_VertexTo()->Get_Quadric();

	quadric_01 += quadricConstraint;
	quadric_02 += quadricConstraint;

	_edge->Get_VertexFrom()->Set_Quadric(quadric_01);
	_edge->Get_VertexTo()->Set_Quadric(quadric_02);
}

f32 ProgressiveMesh::_CalcQuadricError(const glm::mat4x4& _quadric, CProgressiveVertex* _vertex, f32 _triangleArea)
{
	f32 cost = 0;
	f32 result[4];

	const glm::vec3 position = _vertex->Get_Position();

	result[0] = position.x * _quadric[0][0] + position.y * _quadric[1][0] + position.z * _quadric[2][0] + 1 * _quadric[3][0];
	result[1] = position.x * _quadric[0][1] + position.y * _quadric[1][1] + position.z * _quadric[2][1] + 1 * _quadric[3][1];
	result[2] = position.x * _quadric[0][2] + position.y * _quadric[1][2] + position.z * _quadric[2][2] + 1 * _quadric[3][2];
	result[3] = position.x * _quadric[0][3] + position.y * _quadric[1][3] + position.z * _quadric[2][3] + 1 * _quadric[3][3];

	cost = result[0] * position.x + result[1] * position.y + result[2] * position.z + result[3];

	if (_triangleArea != 0)
    {
		cost /= _triangleArea;
	}

	return cost;
}

void ProgressiveMesh::_Contract(std::shared_ptr<CProgressiveEdge> _edge)
{
	if (!_edge->IsValid())
    {
		return;
	}
	_edge->Get_VertexFrom()->Set_Active(false);

	if (m_algorithm == E_SIMPLIFICATION_ALGORITHM_QUADRIC || m_algorithm == E_SIMPLIFICATION_ALGORITHM_QUADRICTRI)
    {
        glm::vec3 newPosition = (_edge->Get_VertexFrom()->Get_Position() + _edge->Get_VertexTo()->Get_Position()) * 0.5f;
		glm::vec2 centerTextureCoordinates = (_edge->Get_VertexFrom()->Get_Texcoord() + _edge->Get_VertexTo()->Get_Texcoord()) * 0.5f;

		_edge->Get_VertexTo()->Set_Position(newPosition);
		_edge->Get_VertexTo()->Set_Texcoord(centerTextureCoordinates);
	}

	if (m_algorithm == E_SIMPLIFICATION_ALGORITHM_QUADRIC || m_algorithm == E_SIMPLIFICATION_ALGORITHM_QUADRICTRI)
    {
		_edge->Get_VertexTo()->Set_Quadric(_edge->Get_VertexTo()->Get_Quadric() + _edge->Get_VertexFrom()->Get_Quadric());
	}

	const std::vector<std::shared_ptr<CProgressiveTriangle>> facesFrom = _edge->Get_VertexFrom()->Get_Faces();
	for (ui32 i = 0; i < facesFrom.size(); ++i)
    {
		std::shared_ptr<CProgressiveTriangle> triangle = facesFrom[i];
		triangle->ReplaceVertex(_edge->Get_VertexFrom(), _edge->Get_VertexTo());
	}

	const std::vector<std::shared_ptr<CProgressiveTriangle>> faces = _edge->Get_Faces();
	for(ui32 i = 0; i < faces.size(); ++i)
    {
		std::shared_ptr<CProgressiveTriangle> triangle = faces[i];
		triangle->Set_Active(false);
        
        std::vector<std::shared_ptr<CProgressiveTriangle>>::iterator iterator = find(m_faces.begin(), m_faces.end(), triangle);
		if (iterator != m_faces.end())
        {
			m_faces.erase(iterator);
		}
	}

	if (m_algorithm == E_SIMPLIFICATION_ALGORITHM_QUADRICTRI)
    {
		_edge->Get_VertexTo()->CalculateQuadric(true);
	}
    else if (m_algorithm == E_SIMPLIFICATION_ALGORITHM_QUADRIC)
    {
		_edge->Get_VertexTo()->CalculateQuadric(false);
	}

	std::map<std::string, std::shared_ptr<CProgressiveEdge>> uniqueEdges;

	for(ui32 i = 0; i < m_edges.size(); ++i)
    {
		std::shared_ptr<CProgressiveEdge> edge = m_edges[i];

		if (edge->HasVertex(_edge->Get_VertexFrom()) || edge->HasVertex(_edge->Get_VertexTo()))
        {
			if (edge->HasVertex(_edge->Get_VertexFrom()))
            {
				edge->ReplaceVertex(_edge->Get_VertexFrom(), _edge->Get_VertexTo());
			}
            else
            {
				edge->Update();
			}

			if (!edge->IsValid())
            {
                std::vector<std::shared_ptr<CProgressiveEdge>>::iterator iterator = find(m_edges.begin(), m_edges.end(), edge);
                if (iterator != m_edges.end())
                {
                    m_edges.erase(iterator);
                }
                else
                {
                    assert(false);
                }
				--i;
			}
			else
            {
                std::string string = edge->Get_IdString();

				std::map<std::string, std::shared_ptr<CProgressiveEdge>>::iterator iterator = uniqueEdges.find(string);
				if (iterator == uniqueEdges.end())
                {
					uniqueEdges.insert(std::make_pair(string, nullptr));
					_CalculateEdgeCost(edge);
				}
				else
                {
					std::vector<std::shared_ptr<CProgressiveEdge>>::iterator iterator = find(m_edges.begin(), m_edges.end(), edge);
                    if (iterator != m_edges.end())
                    {
                        m_edges.erase(iterator);
                    }
                    else
                    {
                        assert(false);
                    }
					--i;
				}
			}
		}
	}
}

void ProgressiveMesh::Contract(ui32 _numberOfEdges)
{
	for(ui32 i = 0; i < _numberOfEdges && m_faces.size() > 0; ++i)
    {
		std::shared_ptr<CProgressiveEdge> edge_01 = m_edges[0];
        std::vector<std::shared_ptr<CProgressiveEdge>>::iterator iterator = m_edges.begin();
		for(ui32 j = 0; j < m_edges.size(); ++j)
        {
			std::shared_ptr<CProgressiveEdge> edge_02 = m_edges[j];
			if (edge_02->Get_Cost() < edge_01->Get_Cost())
            {
				edge_01 = edge_02;
                
                iterator = find(m_edges.begin(), m_edges.end(), edge_02);
                if (iterator == m_edges.end())
                {
                    assert(false);
                }
			}
		}
		m_edges.erase(iterator);
		_Contract(edge_01);
	}
}

void ProgressiveMesh::ContractTillTriangleCount(ui32 _numberOfTriangles)
{
	while (m_faces.size() > _numberOfTriangles)
    {
		Contract(1);
	}
}
*/

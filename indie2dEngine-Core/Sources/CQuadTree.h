//
//  CQuadTree.h
//
//
//  Created by Sergey Sergeev on 3/3/13.
//
//

#ifndef CQuadTree_h
#define CQuadTree_h

#define k_MAX_QUAD_TREE_CHILDS 4

#include "HCommon.h"

class CVertexBuffer;
class CIndexBuffer;
struct SHardwareVertex;
class CFrustum;

class CQuadTree : public std::enable_shared_from_this<CQuadTree>
{
private:
    
    std::shared_ptr<CQuadTree> m_parent;
    std::vector<std::shared_ptr<CQuadTree>> m_childs;

    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;

    ui16* m_indexes;
    ui16* m_indexesIds;
    ui32 m_numIndexes;

    SHardwareVertex* m_vertexes;
    std::shared_ptr<CIndexBuffer> m_indexBuffer;

protected:
    
    void _CreateQuadTreeNode(i32 _size, i32 _depth, const std::shared_ptr<CQuadTree>& _root);
    void _CreateIndexBufferForQuadTreeNode(const std::shared_ptr<CQuadTree>& _node);
    bool _IsPointInBoundBox(const glm::vec3& _point, const glm::vec3& _minBound, const glm::vec3& _maxBound);
    void _GenerateQuadTreeNode(const std::shared_ptr<CFrustum>& _frustum, const std::shared_ptr<CQuadTree>& _root, ui16* _indexes, ui32& _numIndexes);

public:
    
    CQuadTree(void);
    ~CQuadTree(void);

    void Generate(const std::shared_ptr<CVertexBuffer>& _vertexBuffer, const std::shared_ptr<CIndexBuffer>& _indexBuffer, const glm::vec3& _maxBound, const glm::vec3& _minBound, f32 _depth, ui32 _size);
    ui32 OnUpdate(const std::shared_ptr<CFrustum>& _frustum);
};


#endif

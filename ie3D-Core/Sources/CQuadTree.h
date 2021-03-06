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
#include "HDeclaration.h"

class CQuadTree : public std::enable_shared_from_this<CQuadTree>
{
private:
    
    std::weak_ptr<CQuadTree> m_parent;
    std::vector<CSharedQuadTree> m_childs;

    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;

    std::vector<ui16> m_indexes;
    std::vector<ui16> m_indexesIds;
    ui32 m_numIndexes;

    SAttributeVertex* m_vertexes;
    CSharedIndexBuffer m_indexBuffer;
    
    bool m_isGenerated;

protected:
    
    void createQuadTreeNode(i32 size, i32 depth,
                            CSharedQuadTreeRef root);
    
    void createIndexBufferForQuadTreeNode(CSharedQuadTreeRef node);
    
    
    
    void generateQuadTreeNode(CSharedFrustumRef frustum,
                              const glm::mat4& matrixM,
                              CSharedQuadTreeRef root,
                              ui16* indexes,
                              ui32& numIndexes);
    
public:
    
    CQuadTree(void);
    ~CQuadTree(void);
    
    void generate(CSharedVertexBuffer vertexBuffer,
                  CSharedIndexBuffer indexBuffer,
                  const glm::vec3& maxBound,
                  const glm::vec3& minBound,
                  f32 depth,
                  ui32 size);
    
    ui32 update(CSharedFrustumRef frustum, const glm::mat4& matrixM);
    bool getIsGenerated(void) const;
};


#endif

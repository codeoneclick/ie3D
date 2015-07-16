//
//  CQuadTree.cpp
//  iGaia
//
//  Created by Sergey Sergeev on 3/3/13.
//
//

#include "CQuadTree.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CFrustum.h"
#include "CBoundingBox.h"

CQuadTree::CQuadTree(void) :
m_indexBuffer(nullptr), 
m_numIndexes(0),
m_vertexes(nullptr),
m_isGenerated(false)
{

}

CQuadTree::~CQuadTree(void)
{

}

void CQuadTree::createQuadTreeNode(i32 size, i32 depth,
                                   CSharedQuadTreeRef root)
{
    if(size <= depth)
    {
        return;
    }
    
    root->m_childs.resize(k_MAX_QUAD_TREE_CHILDS, nullptr);
    
    root->m_childs.at(0) = std::make_shared<CQuadTree>();
    root->m_childs.at(0)->m_parent = root;
    root->m_childs.at(0)->m_minBound = glm::vec3(root->m_minBound.x,
                                                 root->m_minBound.y,
                                                 root->m_minBound.z );
    root->m_childs.at(0)->m_maxBound = glm::vec3(root->m_minBound.x + (root->m_maxBound.x - root->m_minBound.x) / 2.0,
                                                 root->m_maxBound.y,
                                                 root->m_minBound.z + (root->m_maxBound.z - root->m_minBound.z) / 2.0);
    root->m_childs.at(0)->m_vertexes = m_vertexes;
    CQuadTree::createIndexBufferForQuadTreeNode(root->m_childs.at(0));
    
    root->m_childs.at(1) = std::make_shared<CQuadTree>();
    root->m_childs.at(1)->m_parent = root;
    root->m_childs.at(1)->m_minBound = glm::vec3(root->m_minBound.x,
                                                 root->m_minBound.y,
                                                 root->m_minBound.z + (root->m_maxBound.z - root->m_minBound.z) / 2.0);
    root->m_childs.at(1)->m_maxBound = glm::vec3(root->m_minBound.x + (root->m_maxBound.x - root->m_minBound.x) / 2.0,
                                                 root->m_maxBound.y,
                                                 root->m_maxBound.z);
    root->m_childs.at(1)->m_vertexes = m_vertexes;
    CQuadTree::createIndexBufferForQuadTreeNode(root->m_childs.at(1));
    
    root->m_childs.at(2) = std::make_shared<CQuadTree>();
    root->m_childs.at(2)->m_parent = root;
    root->m_childs.at(2)->m_minBound = glm::vec3(root->m_minBound.x + (root->m_maxBound.x - root->m_minBound.x) / 2.0,
                                                 root->m_minBound.y,
                                                 root->m_minBound.z + (root->m_maxBound.z - root->m_minBound.z) / 2.0);
    root->m_childs.at(2)->m_maxBound = glm::vec3(root->m_maxBound.x,
                                                 root->m_maxBound.y,
                                                 root->m_maxBound.z);
    root->m_childs.at(2)->m_vertexes = m_vertexes;
    CQuadTree::createIndexBufferForQuadTreeNode(root->m_childs.at(2));
    
    root->m_childs.at(3) = std::make_shared<CQuadTree>();
    root->m_childs.at(3)->m_parent = root;
    root->m_childs.at(3)->m_minBound = glm::vec3(root->m_minBound.x + (root->m_maxBound.x - root->m_minBound.x) / 2.0,
                                                 root->m_minBound.y,
                                                 root->m_minBound.z);
    root->m_childs.at(3)->m_maxBound = glm::vec3(root->m_maxBound.x,
                                                 root->m_maxBound.y,
                                                 root->m_minBound.z + (root->m_maxBound.z - root->m_minBound.z) / 2.0);
    root->m_childs.at(3)->m_vertexes = m_vertexes;
    CQuadTree::createIndexBufferForQuadTreeNode(root->m_childs.at(3));
    
    CQuadTree::createQuadTreeNode(size / 2, depth, root->m_childs.at(0));
    CQuadTree::createQuadTreeNode(size / 2, depth, root->m_childs.at(1));
    CQuadTree::createQuadTreeNode(size / 2, depth, root->m_childs.at(2));
    CQuadTree::createQuadTreeNode(size / 2, depth, root->m_childs.at(3));
}

void CQuadTree::createIndexBufferForQuadTreeNode(CSharedQuadTreeRef node)
{
    CSharedQuadTree parent = node->m_parent.lock();
    ui32 parentNumIndexes = parent->m_numIndexes;
    f32 maxY = -4096.0f;
    f32 minY =  4096.0f;
    
    ui32 quadTreeNodeId = 0;
    while (parent != nullptr)
    {
        quadTreeNodeId++;
        parent = parent->m_parent.lock();
    }
    parent = node->m_parent.lock();
    
    for(ui32 i = 0; i < parentNumIndexes; i += 3)
    {
        if(CBoundingBox::isPointInBoundBox(glm::vec3(m_vertexes[parent->m_indexes[i + 0]].m_position.x,
                                                     m_vertexes[parent->m_indexes[i + 0]].m_position.y,
                                                     m_vertexes[parent->m_indexes[i + 0]].m_position.z) ,
                                           node->m_minBound,
                                           node->m_maxBound) ||
           CBoundingBox::isPointInBoundBox(glm::vec3(m_vertexes[parent->m_indexes[i + 1]].m_position.x,
                                                     m_vertexes[parent->m_indexes[i + 1]].m_position.y,
                                                     m_vertexes[parent->m_indexes[i + 1]].m_position.z),
                                           node->m_minBound,
                                           node->m_maxBound) ||
           CBoundingBox::isPointInBoundBox(glm::vec3(m_vertexes[parent->m_indexes[i + 2]].m_position.x,
                                                     m_vertexes[parent->m_indexes[i + 2]].m_position.y,
                                                     m_vertexes[parent->m_indexes[i + 2]].m_position.z),
                                           node->m_minBound,
                                           node->m_maxBound))
        {
            if(parent->m_indexesIds[i + 0] == quadTreeNodeId ||
               parent->m_indexesIds[i + 1] == quadTreeNodeId ||
               parent->m_indexesIds[i + 2] == quadTreeNodeId)
            {
                continue;
            }
            
            node->m_numIndexes += 3;
            node->m_indexes.resize(node->m_numIndexes);
            
            node->m_indexes[node->m_numIndexes - 3] = parent->m_indexes[i + 0];
            node->m_indexes[node->m_numIndexes - 2] = parent->m_indexes[i + 1];
            node->m_indexes[node->m_numIndexes - 1] = parent->m_indexes[i + 2];
            
            parent->m_indexesIds[i + 0] = quadTreeNodeId;
            parent->m_indexesIds[i + 1] = quadTreeNodeId;
            parent->m_indexesIds[i + 2] = quadTreeNodeId;
            
            if(m_vertexes[parent->m_indexes[i + 0]].m_position.y > maxY)
            {
                maxY = m_vertexes[parent->m_indexes[i + 0]].m_position.y;
            }
            
            if(m_vertexes[parent->m_indexes[i + 1]].m_position.y > maxY)
            {
                maxY = m_vertexes[parent->m_indexes[i + 1]].m_position.y;
            }
            
            if(m_vertexes[parent->m_indexes[i + 2]].m_position.y > maxY)
            {
                maxY = m_vertexes[parent->m_indexes[i + 2]].m_position.y;
            }
            
            if(m_vertexes[parent->m_indexes[i + 0]].m_position.y < minY)
            {
                minY = m_vertexes[parent->m_indexes[i + 0]].m_position.y;
            }
            
            if(m_vertexes[parent->m_indexes[i + 1]].m_position.y < minY)
            {
                minY = m_vertexes[parent->m_indexes[i + 1]].m_position.y;
            }
            
            if(m_vertexes[parent->m_indexes[i + 2]].m_position.y < minY)
            {
                minY = m_vertexes[parent->m_indexes[i + 2]].m_position.y;
            }
        }
    }
    node->m_indexesIds.resize(node->m_numIndexes);
    node->m_maxBound.y = maxY;
    node->m_minBound.y = minY;
}

void CQuadTree::generateQuadTreeNode(CSharedFrustumRef frustum,
                                     const glm::mat4& matrixM,
                                     CSharedQuadTreeRef root,
                                     ui16* indexes,
                                     ui32& numIndexes)
{
    if(root->m_childs.size() == 0)
    {
        return;
    }
    
    for(ui32 i = 0; i < k_MAX_QUAD_TREE_CHILDS; i++)
    {
        i32 result = frustum->isBoundBoxInFrustum(root->m_childs.at(i)->m_maxBound,
                                                  root->m_childs.at(i)->m_minBound,
                                                  matrixM);
        if(result == E_FRUSTUM_BOUND_RESULT_INSIDE)
        {
            std::memcpy(&indexes[numIndexes], &root->m_childs[i]->m_indexes[0], sizeof(ui16) * root->m_childs[i]->m_numIndexes);
            numIndexes += root->m_childs[i]->m_numIndexes;
        }
        else if(result == E_FRUSTUM_BOUND_RESULT_INTERSECT)
        {
            if(root->m_childs[i]->m_childs.size() == 0)
            {
                std::memcpy(&indexes[numIndexes], &root->m_childs[i]->m_indexes[0], sizeof(ui16) * root->m_childs[i]->m_numIndexes);
                numIndexes += root->m_childs[i]->m_numIndexes;
            }
            else
            {
                CQuadTree::generateQuadTreeNode(frustum, matrixM, root->m_childs[i], indexes, numIndexes);
            }
        }
    }
}

void CQuadTree::generate(CSharedVertexBuffer vertexBuffer,
                         CSharedIndexBuffer indexBuffer,
                         const glm::vec3& maxBound,
                         const glm::vec3& minBound,
                         f32 depth,
                         ui32 size)
{
    assert(m_indexBuffer == nullptr);
    assert(m_vertexes == nullptr);
    
    m_indexBuffer = indexBuffer;
    m_maxBound = maxBound;
    m_minBound = minBound;
    m_numIndexes = m_indexBuffer->getUsedSize();
    m_indexes.resize(m_numIndexes);
    m_indexesIds.resize(m_numIndexes);
    ui16* indexData = indexBuffer->lock();
    std::copy(indexData, indexData + m_numIndexes, m_indexes.begin());
    m_vertexes = vertexBuffer->lock();
    CSharedQuadTree root = shared_from_this();
    CQuadTree::createQuadTreeNode(size, depth, root);
    
    assert(m_indexBuffer != nullptr);
    assert(m_childs.at(0) != nullptr);
    assert(m_childs.at(1) != nullptr);
    assert(m_childs.at(2) != nullptr);
    assert(m_childs.at(3) != nullptr);
    m_isGenerated = true;
}

ui32 CQuadTree::update(CSharedFrustumRef frustum, const glm::mat4& matrixM)
{
    ui32 numIndexes = 0;
    if(m_isGenerated)
    {
        ui16* indexes = m_indexBuffer->lock();
        CSharedQuadTree root = shared_from_this();
        CQuadTree::generateQuadTreeNode(frustum, matrixM, root, indexes, numIndexes);
        m_indexBuffer->unlock(numIndexes);
    }
    return numIndexes;
}

bool CQuadTree::getIsGenerated(void) const
{
    return m_isGenerated;
}

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

CQuadTree::CQuadTree(void) :
m_parent(nullptr),
m_indexes(nullptr),
m_indexBuffer(nullptr), 
m_numIndexes(0)
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
    
    root->m_childs.resize(k_MAX_QUAD_TREE_CHILDS);
    
    root->m_childs[0] = std::make_shared<CQuadTree>();
    root->m_childs[0]->m_parent = root;
    root->m_childs[0]->m_minBound = glm::vec3(root->m_minBound.x,
                                              root->m_minBound.y,
                                              root->m_minBound.z );
    root->m_childs[0]->m_maxBound = glm::vec3(root->m_minBound.x + (root->m_maxBound.x - root->m_minBound.x) / 2.0f,
                                              root->m_maxBound.y,
                                              root->m_minBound.z + (root->m_maxBound.z - root->m_minBound.z) / 2.0f);
    root->m_childs[0]->m_vertexes = m_vertexes;
    CQuadTree::createIndexBufferForQuadTreeNode(root->m_childs[0]);
    
    root->m_childs[1] = std::make_shared<CQuadTree>();
    root->m_childs[1]->m_parent = root;
    root->m_childs[1]->m_minBound = glm::vec3(root->m_minBound.x,
                                              root->m_minBound.y,
                                              root->m_minBound.z + (root->m_maxBound.z - root->m_minBound.z) / 2.0f);
    root->m_childs[1]->m_maxBound = glm::vec3(root->m_minBound.x + (root->m_maxBound.x - root->m_minBound.x) / 2.0f,
                                              root->m_maxBound.y,
                                              root->m_maxBound.z);
    root->m_childs[1]->m_vertexes = m_vertexes;
    CQuadTree::createIndexBufferForQuadTreeNode(root->m_childs[1]);
    
    root->m_childs[2] = std::make_shared<CQuadTree>();
    root->m_childs[2]->m_parent = root;
    root->m_childs[2]->m_minBound = glm::vec3(root->m_minBound.x + (root->m_maxBound.x - root->m_minBound.x) / 2.0f,
                                              root->m_minBound.y,
                                              root->m_minBound.z + (root->m_maxBound.z - root->m_minBound.z) / 2.0f);
    root->m_childs[2]->m_maxBound = glm::vec3(root->m_maxBound.x,
                                               root->m_maxBound.y,
                                               root->m_maxBound.z);
    root->m_childs[2]->m_vertexes = m_vertexes;
    CQuadTree::createIndexBufferForQuadTreeNode(root->m_childs[2]);
    
    root->m_childs[3] = std::make_shared<CQuadTree>();
    root->m_childs[3]->m_parent = root;
    root->m_childs[3]->m_minBound = glm::vec3(root->m_minBound.x + (root->m_maxBound.x - root->m_minBound.x) / 2.0f,
                                              root->m_minBound.y,
                                              root->m_minBound.z);
    root->m_childs[3]->m_maxBound = glm::vec3(root->m_maxBound.x,
                                              root->m_maxBound.y,
                                              root->m_minBound.z + (root->m_maxBound.z - root->m_minBound.z) / 2.0f);
    root->m_childs[3]->m_vertexes = m_vertexes;
    CQuadTree::createIndexBufferForQuadTreeNode(root->m_childs[3]);
    
    CQuadTree::createQuadTreeNode(size / 2, depth, root->m_childs[0]);
    CQuadTree::createQuadTreeNode(size / 2, depth, root->m_childs[1]);
    CQuadTree::createQuadTreeNode(size / 2, depth, root->m_childs[2]);
    CQuadTree::createQuadTreeNode(size / 2, depth, root->m_childs[3]);
}

void CQuadTree::createIndexBufferForQuadTreeNode(CSharedQuadTreeRef node)
{
    ui32 parentNumIndexes = node->m_parent->m_numIndexes;
    node->m_indexes = static_cast<ui16*>(malloc(sizeof(ui16)));
    f32 maxY = -4096.0f;
    f32 minY =  4096.0f;
    
    ui32 quadTreeNodeId = 0;
    std::shared_ptr<CQuadTree> parentNode = node->m_parent;
    while (parentNode != nullptr)
    {
        quadTreeNodeId++;
        parentNode = parentNode->m_parent;
    }
    
    for(ui32 i = 0; i < parentNumIndexes; i += 3)
    {
        if(CQuadTree::isPointInBoundBox(glm::vec3(m_vertexes[node->m_parent->m_indexes[i + 0]].m_position.x,
                                                  m_vertexes[node->m_parent->m_indexes[i + 0]].m_position.y,
                                                  m_vertexes[node->m_parent->m_indexes[i + 0]].m_position.z) ,
                                        node->m_minBound,
                                        node->m_maxBound) ||
           CQuadTree::isPointInBoundBox(glm::vec3(m_vertexes[node->m_parent->m_indexes[i + 1]].m_position.x,
                                                  m_vertexes[node->m_parent->m_indexes[i + 1]].m_position.y,
                                                  m_vertexes[node->m_parent->m_indexes[i + 1]].m_position.z),
                                        node->m_minBound,
                                        node->m_maxBound) ||
           CQuadTree::isPointInBoundBox(glm::vec3(m_vertexes[node->m_parent->m_indexes[i + 2]].m_position.x,
                                                  m_vertexes[node->m_parent->m_indexes[i + 2]].m_position.y,
                                                  m_vertexes[node->m_parent->m_indexes[i + 2]].m_position.z),
                                        node->m_minBound,
                                        node->m_maxBound))
        {
            
            if(node->m_parent->m_indexesIds[i + 0] == quadTreeNodeId ||
               node->m_parent->m_indexesIds[i + 1] == quadTreeNodeId ||
               node->m_parent->m_indexesIds[i + 2] == quadTreeNodeId)
            {
                continue;
            }
            
            node->m_numIndexes += 3;
            node->m_indexes = static_cast<ui16*>(realloc(node->m_indexes, sizeof(ui16) * node->m_numIndexes));
            
            node->m_indexes[node->m_numIndexes - 3] = node->m_parent->m_indexes[i + 0];
            node->m_indexes[node->m_numIndexes - 2] = node->m_parent->m_indexes[i + 1];
            node->m_indexes[node->m_numIndexes - 1] = node->m_parent->m_indexes[i + 2];
            
            node->m_parent->m_indexesIds[i + 0] = quadTreeNodeId;
            node->m_parent->m_indexesIds[i + 1] = quadTreeNodeId;
            node->m_parent->m_indexesIds[i + 2] = quadTreeNodeId;
            
            if(m_vertexes[node->m_parent->m_indexes[i + 0]].m_position.y > maxY)
            {
                maxY = m_vertexes[node->m_parent->m_indexes[i + 0]].m_position.y;
            }
            
            if(m_vertexes[node->m_parent->m_indexes[i + 1]].m_position.y > maxY)
            {
                maxY = m_vertexes[node->m_parent->m_indexes[i + 1]].m_position.y;
            }
            
            if(m_vertexes[node->m_parent->m_indexes[i + 2]].m_position.y > maxY)
            {
                maxY = m_vertexes[node->m_parent->m_indexes[i + 2]].m_position.y;
            }
            
            if(m_vertexes[node->m_parent->m_indexes[i + 0]].m_position.y < minY)
            {
                minY = m_vertexes[node->m_parent->m_indexes[i + 0]].m_position.y;
            }
            
            if(m_vertexes[node->m_parent->m_indexes[i + 1]].m_position.y < minY)
            {
                minY = m_vertexes[node->m_parent->m_indexes[i + 1]].m_position.y;
            }
            
            if(m_vertexes[node->m_parent->m_indexes[i + 2]].m_position.y < minY)
            {
                minY = m_vertexes[node->m_parent->m_indexes[i + 2]].m_position.y;
            }
        }
    }
    node->m_indexesIds = static_cast<ui16*>(malloc(node->m_numIndexes * sizeof(ui16)));
    memset(node->m_indexesIds, 0x0, node->m_numIndexes * sizeof(ui16));
    node->m_maxBound.y = maxY;
    node->m_minBound.y = minY;
}

bool CQuadTree::isPointInBoundBox(const glm::vec3& point,
                       const glm::vec3& minBound,
                       const glm::vec3& maxBound)
{
    if(point.x >= minBound.x &&
       point.x <= maxBound.x &&
       point.y >= minBound.y &&
       point.y <= maxBound.y &&
       point.z >= minBound.z &&
       point.z <= maxBound.z)
    {
        return true;
    }
    return false;
}

void CQuadTree::generateQuadTreeNode(CSharedFrustumRef frustum,
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
        i32 result =  frustum->IsBoundBoxInFrustum(root->m_childs[i]->m_maxBound,
                                                   root->m_childs[i]->m_minBound);
        if(result == E_FRUSTUM_BOUND_RESULT_INSIDE)
        {
            memcpy(&indexes[numIndexes], root->m_childs[i]->m_indexes, sizeof(ui16) * root->m_childs[i]->m_numIndexes);
            numIndexes += root->m_childs[i]->m_numIndexes;
        }
        else if(result == E_FRUSTUM_BOUND_RESULT_INTERSECT)
        {
            if(root->m_childs[i]->m_childs.size() == 0)
            {
                memcpy(&indexes[numIndexes], root->m_childs[i]->m_indexes, sizeof(ui16) * root->m_childs[i]->m_numIndexes);
                numIndexes += root->m_childs[i]->m_numIndexes;
            }
            else
            {
                CQuadTree::generateQuadTreeNode(frustum, root->m_childs[i], indexes, numIndexes);
            }
        }
    }
}

void CQuadTree::generate(CSharedVertexBufferRef vertexBuffer,
              CSharedIndexBufferRef indexBuffer,
              const glm::vec3& maxBound,
              const glm::vec3& minBound,
              f32 depth,
              ui32 size)
{
    m_indexBuffer = indexBuffer;
    m_parent = nullptr;
    m_maxBound = maxBound;
    m_minBound = minBound;
    m_numIndexes = m_indexBuffer->getSize();
    m_indexes = static_cast<ui16*>(malloc(m_numIndexes * sizeof(ui16)));
    m_indexesIds = static_cast<ui16*>(malloc(m_numIndexes * sizeof(ui16)));
    ui16* indexData = indexBuffer->lock();
    memcpy(m_indexes , indexData, m_numIndexes * sizeof(ui16));
    memset(m_indexesIds, 0x0, m_numIndexes * sizeof(ui16));
    m_vertexes = vertexBuffer->lock();
    CQuadTree::createQuadTreeNode(size, depth, shared_from_this());
}

ui32 CQuadTree::update(CSharedFrustumRef frustum)
{
    ui16* indexes = m_indexBuffer->lock();
    ui32 numIndexes = 0;
    CQuadTree::generateQuadTreeNode(frustum, shared_from_this(), indexes, numIndexes);
    m_indexBuffer->unlock(numIndexes);
    return numIndexes;
}


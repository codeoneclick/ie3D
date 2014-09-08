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

static ui32 allocations = 0;

CQuadTree::CQuadTree(void) :
m_indexBuffer(nullptr), 
m_numIndexes(0)
{
    allocations++;
    std::cout<<"allocations: "<<allocations<<std::endl;
}

CQuadTree::~CQuadTree(void)
{
    std::vector<CSharedQuadTree> childsDeleter;
    m_childs.swap(childsDeleter);
    
    std::vector<ui16> indexesDeleter;
    m_indexes.swap(indexesDeleter);
    m_indexesIds.swap(indexesDeleter);
    
    allocations--;
    std::cout<<"allocations: "<<allocations<<std::endl;
}

void CQuadTree::createQuadTreeNode(i32 size, i32 depth,
                                   CSharedQuadTreeRef root)
{
    if(size <= depth)
    {
        return;
    }
    
    root->m_childs.resize(k_MAX_QUAD_TREE_CHILDS);
    
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
    ui32 parentNumIndexes = node->m_parent.lock()->m_numIndexes;
    f32 maxY = -4096.0f;
    f32 minY =  4096.0f;
    
    ui32 quadTreeNodeId = 0;
    std::shared_ptr<CQuadTree> parentNode = node->m_parent.lock();
    while (parentNode != nullptr)
    {
        quadTreeNodeId++;
        parentNode = parentNode->m_parent.lock();
    }
    
    for(ui32 i = 0; i < parentNumIndexes; i += 3)
    {
        if(CQuadTree::isPointInBoundBox(glm::vec3(m_vertexes[node->m_parent.lock()->m_indexes[i + 0]].m_position.x,
                                                  m_vertexes[node->m_parent.lock()->m_indexes[i + 0]].m_position.y,
                                                  m_vertexes[node->m_parent.lock()->m_indexes[i + 0]].m_position.z) ,
                                        node->m_minBound,
                                        node->m_maxBound) ||
           CQuadTree::isPointInBoundBox(glm::vec3(m_vertexes[node->m_parent.lock()->m_indexes[i + 1]].m_position.x,
                                                  m_vertexes[node->m_parent.lock()->m_indexes[i + 1]].m_position.y,
                                                  m_vertexes[node->m_parent.lock()->m_indexes[i + 1]].m_position.z),
                                        node->m_minBound,
                                        node->m_maxBound) ||
           CQuadTree::isPointInBoundBox(glm::vec3(m_vertexes[node->m_parent.lock()->m_indexes[i + 2]].m_position.x,
                                                  m_vertexes[node->m_parent.lock()->m_indexes[i + 2]].m_position.y,
                                                  m_vertexes[node->m_parent.lock()->m_indexes[i + 2]].m_position.z),
                                        node->m_minBound,
                                        node->m_maxBound))
        {
            
            if(node->m_parent.lock()->m_indexesIds[i + 0] == quadTreeNodeId ||
               node->m_parent.lock()->m_indexesIds[i + 1] == quadTreeNodeId ||
               node->m_parent.lock()->m_indexesIds[i + 2] == quadTreeNodeId)
            {
                continue;
            }
            
            node->m_numIndexes += 3;
            node->m_indexes.resize(node->m_numIndexes);
            
            node->m_indexes[node->m_numIndexes - 3] = node->m_parent.lock()->m_indexes[i + 0];
            node->m_indexes[node->m_numIndexes - 2] = node->m_parent.lock()->m_indexes[i + 1];
            node->m_indexes[node->m_numIndexes - 1] = node->m_parent.lock()->m_indexes[i + 2];
            
            node->m_parent.lock()->m_indexesIds[i + 0] = quadTreeNodeId;
            node->m_parent.lock()->m_indexesIds[i + 1] = quadTreeNodeId;
            node->m_parent.lock()->m_indexesIds[i + 2] = quadTreeNodeId;
            
            if(m_vertexes[node->m_parent.lock()->m_indexes[i + 0]].m_position.y > maxY)
            {
                maxY = m_vertexes[node->m_parent.lock()->m_indexes[i + 0]].m_position.y;
            }
            
            if(m_vertexes[node->m_parent.lock()->m_indexes[i + 1]].m_position.y > maxY)
            {
                maxY = m_vertexes[node->m_parent.lock()->m_indexes[i + 1]].m_position.y;
            }
            
            if(m_vertexes[node->m_parent.lock()->m_indexes[i + 2]].m_position.y > maxY)
            {
                maxY = m_vertexes[node->m_parent.lock()->m_indexes[i + 2]].m_position.y;
            }
            
            if(m_vertexes[node->m_parent.lock()->m_indexes[i + 0]].m_position.y < minY)
            {
                minY = m_vertexes[node->m_parent.lock()->m_indexes[i + 0]].m_position.y;
            }
            
            if(m_vertexes[node->m_parent.lock()->m_indexes[i + 1]].m_position.y < minY)
            {
                minY = m_vertexes[node->m_parent.lock()->m_indexes[i + 1]].m_position.y;
            }
            
            if(m_vertexes[node->m_parent.lock()->m_indexes[i + 2]].m_position.y < minY)
            {
                minY = m_vertexes[node->m_parent.lock()->m_indexes[i + 2]].m_position.y;
            }
        }
    }
    node->m_indexesIds.resize(node->m_numIndexes);
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
        i32 result = frustum->IsBoundBoxInFrustum(root->m_childs.at(i)->m_maxBound,
                                                  root->m_childs.at(i)->m_minBound);
        if(result == E_FRUSTUM_BOUND_RESULT_INSIDE)
        {
            memcpy(&indexes[numIndexes], &root->m_childs[i]->m_indexes[0], sizeof(ui16) * root->m_childs[i]->m_numIndexes);
            numIndexes += root->m_childs[i]->m_numIndexes;
        }
        else if(result == E_FRUSTUM_BOUND_RESULT_INTERSECT)
        {
            if(root->m_childs[i]->m_childs.size() == 0)
            {
                memcpy(&indexes[numIndexes], &root->m_childs[i]->m_indexes[0], sizeof(ui16) * root->m_childs[i]->m_numIndexes);
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
    m_maxBound = maxBound;
    m_minBound = minBound;
    m_numIndexes = m_indexBuffer->getSize();
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
}

ui32 CQuadTree::update(CSharedFrustumRef frustum)
{
    ui16* indexes = m_indexBuffer->lock();
    ui32 numIndexes = 0;
    CSharedQuadTree root = shared_from_this();
    CQuadTree::generateQuadTreeNode(frustum, root, indexes, numIndexes);
    m_indexBuffer->unlock(numIndexes);
    return numIndexes;
}


//
//  CBone.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CBone_h
#define CBone_h

#include "HCommon.h"
#include "HDeclaration.h"

class CBone final : public std::enable_shared_from_this<CBone>
{
private:
    
protected:
    
	i32	m_id;
	i32	m_parentId;

    CSharedBone m_parent;
    std::vector<CSharedBone> m_children;
   
    glm::mat4  m_baseTransformation;
    glm::mat4* m_transformation;
    glm::mat4  m_bindTransformation;
    
public:
    
    CBone(i32 id, i32 parentId);
    ~CBone(void);
    
    void addChild(CSharedBoneRef bone);
    CSharedBone findChild(i32 id);
    
    const std::vector<CSharedBone>& getChildren(void) const;
    CSharedBone getParent(void) const;
    
    i32 getId(void) const;
    i32 getParentId(void) const;
    
    void setTransformation(glm::mat4* transformation);
	glm::mat4* getTransformation(void) const;
    glm::mat4 getBaseTransformation(void) const;

    void update(void);
    void bindPoseTransformation(void);
};

#endif 



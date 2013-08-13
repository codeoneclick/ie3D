//
//  CSkeleton.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CSkeleton_h
#define CSkeleton_h

#include "HCommon.h"
#include "IResource.h"

class CBone;
class CVertexBuffer;
class CIndexBuffer;

class CSkeleton final : public IResource
{
private:
    
    friend class CSkeletonSerializer_SK;
    friend class CSkeletonCommiter_SK;
    
protected:
    
    std::shared_ptr<CVertexBuffer> m_vertexBuffer;
    std::shared_ptr<CIndexBuffer> m_indexBuffer;
    f32 m_boneWidth;
    
    i32	m_numBones;
    std::set<std::shared_ptr<CBone> > m_roots;
    glm::mat4* m_bonesTransformation;
    
    void _Serialize(std::ifstream& _stream);
    void _BindSkeleton(void);
    
public:
    
    CSkeleton(const std::string& _guid);
    ~CSkeleton(void);
    
    void AddBone(std::shared_ptr<CBone> _bone);
    std::shared_ptr<CBone> Get_BoneById(i32 _id);
	void Update(void);
	void Set_BindTransformation(void);
    
    void Draw(const i32* _attributes);
    
    inline i32 Get_NumBones(void)
    {
        return m_numBones;
    };
    
    inline glm::mat4* Get_Transformations(void)
    {
        return m_bonesTransformation;
    };
};

#endif


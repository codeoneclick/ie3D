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
class CHVertexBuffer;
class CSVertexBuffer;
class CHIndexBuffer;
class CSIndexBuffer;

class CSkeleton final : public IResource
{
private:
    
    friend class CSkeletonSerializer_SK;
    friend class CSkeletonCommiter_SK;
    
protected:
    
    f32 m_boneWidth;
    
    ui32 m_numBones;
    std::set<std::shared_ptr<CBone> > m_roots;
    std::map<std::string, glm::mat4x4*> m_transformations;
    
    void _Serialize(std::istream* _stream);
    void _BindSkeleton(void);
    
    static const std::string _GenerateGuid(void);
    
public:
    
    CSkeleton(const std::string& _guid);
    ~CSkeleton(void);
    
    void BindTransformation(void);
    
    void AddBone(std::shared_ptr<CBone> _bone);
    std::shared_ptr<CBone> Get_Bone(i32 _id);
    
    inline ui32 Get_NumBones(void)
    {
        return m_numBones;
    };
    
    void Update(const std::string& _guid);
	void Draw(const i32* _attributes, const std::string& _guid);
    
    std::string CreateTransformations(void);
    glm::mat4* Get_Transformations(const std::string& _guid);
};

#endif


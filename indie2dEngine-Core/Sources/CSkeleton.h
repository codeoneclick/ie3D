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

class CBone;
class CVertexBuffer;
class CIndexBuffer;

class CSkeleton final
{
private:
    
    friend class CMeshSerializer_MDL;
    
protected:
    
    std::shared_ptr<CVertexBuffer> m_vertexBuffer;
    std::shared_ptr<CIndexBuffer> m_indexBuffer;
    f32 m_boneWidth;
    
    i32	m_numBones;
    std::set<std::shared_ptr<CBone> > m_roots;
    
    void _Serialize(std::ifstream& _stream);
    
public:
    
    CSkeleton(void);
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
};

#endif


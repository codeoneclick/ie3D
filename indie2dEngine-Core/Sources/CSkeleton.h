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

class CSkeleton final
{
private:
    
    friend class CMeshSerializer_MDL;
    
protected:
    
    i32	m_numBones;
	std::shared_ptr<CBone> m_root;
    
    void _Serialize(std::ifstream& _stream);
    
public:
    
    CSkeleton(void);
    ~CSkeleton(void);
    
    void AddBone(std::shared_ptr<CBone> _bone);
    std::shared_ptr<CBone> Get_BoneById(i32 _id);
	void AnimateHierarhy(void);
	void SetupBindPosition(void);
    
    inline i32 Get_NumBones(void)
    {
        return m_numBones;
    };
};

#endif


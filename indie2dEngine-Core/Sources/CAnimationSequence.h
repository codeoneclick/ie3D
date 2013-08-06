//
//  CAnimationSequence.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CAnimationSequence_h
#define CAnimationSequence_h

#include "HCommon.h"

class CAnimationFrame final
{
private:
    
    friend class CAnimationSequence;
    
protected:
    
    std::vector<glm::quat> m_rotations;
	std::vector<glm::vec3> m_positions;
    std::vector<glm::vec3> m_scales;
    std::vector<glm::vec3> m_eulers;
    std::vector<glm::vec3> m_eulersYPR;
	i32	m_numBones;
    
    void _Serialize(std::ifstream& _stream);
    
public:
    
    CAnimationFrame(i32 _numBones);
    ~CAnimationFrame(void);
    
    inline const glm::quat& Get_Rotation(i32 _index) const
	{
        assert(_index < m_rotations.size());
		return m_rotations[_index];
	};
    
	inline const glm::vec3& Get_Position(i32 _index) const
	{
        assert(_index < m_positions.size());
		return m_positions[_index];
	};
    
    inline const glm::vec3& Get_Scale(i32 _index) const
	{
        assert(_index < m_scales.size());
		return m_scales[_index];
	};
    
    inline const glm::vec3& Get_Euler(i32 _index) const
	{
        assert(_index < m_eulers.size());
		return m_eulers[_index];
	};
    
    inline const glm::vec3& Get_EulerYPR(i32 _index) const
	{
        assert(_index < m_eulersYPR.size());
		return m_eulersYPR[_index];
	};
};

class CAnimationSequence final
{
private:
    
    friend class CMeshSerializer_MDL;
    
protected:
    
    std::vector<std::shared_ptr<CAnimationFrame> > m_frames;
    void _Serialize(std::ifstream& _stream, i32 _numBones);
    
public:
    
    CAnimationSequence(void);
    ~CAnimationSequence(void);
    
    inline i32 Get_NumFrames(void) const
	{
		return m_frames.size();
	};
    
	inline std::shared_ptr<CAnimationFrame> Get_AnimationFrame(i32 _index) const
	{
		return m_frames[_index];
	};
};

#endif 


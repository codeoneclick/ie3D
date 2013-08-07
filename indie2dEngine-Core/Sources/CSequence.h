//
//  CSequence.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CSequence_h
#define CSequence_h

#include "HCommon.h"

class CFrame final
{
private:
    
    friend class CSequence;
    
protected:
    
    std::vector<glm::quat> m_rotations;
	std::vector<glm::vec3> m_positions;
    std::vector<glm::vec3> m_scales;
	i32	m_numBones;
    
    void _Serialize(std::ifstream& _stream);
    
public:
    
    CFrame(i32 _numBones);
    ~CFrame(void);
    
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
};

class CSequence final
{
private:
    
    friend class CMeshSerializer_MDL;
    
protected:
    
    std::vector<std::shared_ptr<CFrame> > m_frames;
    i32 m_fps;
    void _Serialize(std::ifstream& _stream, i32 _numBones);
    
public:
    
    CSequence(void);
    ~CSequence(void);
    
    inline i32 Get_NumFrames(void) const
	{
		return m_frames.size();
	};
    
    inline i32 Get_Fps(void)
    {
        return m_fps;
    };
    
	inline std::shared_ptr<CFrame> Get_AnimationFrame(i32 _index) const
	{
		return m_frames[_index];
	};
};

#endif 


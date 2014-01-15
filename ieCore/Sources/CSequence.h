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
#include "IResource.h"

class CFrame final 
{
private:
    
    friend class CSequence;
    
protected:
    
    std::vector<glm::quat> m_rotations;
	std::vector<glm::vec3> m_positions;
    std::vector<glm::vec3> m_scales;
	i32	m_numBones;
    
    void _Serialize(std::istream* _stream);
    
public:
    
    CFrame(i32 _numBones);
    ~CFrame(void);
    
    inline const glm::quat& Get_Rotation(ui32 _index) const
	{
        assert(_index < m_rotations.size());
		return m_rotations[_index];
	};
    
	inline const glm::vec3& Get_Position(ui32 _index) const
	{
        assert(_index < m_positions.size());
		return m_positions[_index];
	};
    
    inline const glm::vec3& Get_Scale(ui32 _index) const
	{
        assert(_index < m_scales.size());
		return m_scales[_index];
	};
};


class CSequence final : public IResource
{
private:
    
    friend class CSequenceSerializer_SEQ;
    friend class CSequenceCommiter_SEQ;
    
protected:
    
    std::vector<std::shared_ptr<CFrame> > m_frames;
    i32 m_fps;
    std::string m_name;
    void _Serialize(std::istream* _stream);
    void _BindSequence(void);
    
public:
    
    CSequence(const std::string& _guid);
    ~CSequence(void);
    
    inline i32 Get_NumFrames(void) const
	{
		return m_frames.size();
	};
    
    inline const i32 Get_Fps(void) const
    {
        return m_fps;
    };
    
    inline void Set_Name(const std::string _name)
    {
        m_name = _name;
    };
    
    inline const std::string Get_Name(void) const
    {
        return m_name;
    };
    
	inline std::shared_ptr<CFrame> Get_AnimationFrame(i32 _index) const
	{
		return m_frames[_index];
	};
};

#endif 


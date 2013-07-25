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

class CBone final : public std::enable_shared_from_this<CBone>
{
private:
    
protected:
    
    std::string	m_name;
	i32	m_id;
	i32	m_parentId;
    
    std::shared_ptr<CBone> m_parent;
    std::shared_ptr<CBone> m_next;
	std::shared_ptr<CBone> m_child;
   
    glm::mat4x4 m_transformation;
    glm::mat4x4 m_bindPosition;
    
public:
    
    CBone(const std::string& _name, i32 _id, i32 _parentId);
    ~CBone(void);
    
    void LinkChildBone(std::shared_ptr<CBone> _bone);
    std::shared_ptr<CBone> FindInChildrenById(i32 _id);
    void AnimateHierarhy(const glm::mat4x4& _transformation);
	void SetupBindPosition(void);
    
    inline i32 Get_Id(void) const
	{
		return m_id;
	};
    
	inline int Get_ParentId(void) const
	{
		return m_parentId;
	};
    
	inline std::shared_ptr<CBone> Get_Parent(void) const
	{
		return m_parent;
	};
    
	inline std::shared_ptr<CBone> Get_Next(void) const
	{
		return m_next;
	};
    
	inline std::shared_ptr<CBone> Get_Child(void) const
	{
		return m_child;
	};
    
	inline void Set_BindPosition(const glm::mat4x4& _matrix)
	{
		m_bindPosition = _matrix;
	};
    
	inline const glm::mat4x4& Get_BindPosition(void) const
	{
		return m_bindPosition;
	};
    
	inline void Set_Transformation(const glm::mat4x4& _transformation)
	{
		m_transformation = _transformation;
	};
    
	inline glm::mat4x4 Get_Transformation(void) const
	{
		return m_transformation;
	};
};

#endif 



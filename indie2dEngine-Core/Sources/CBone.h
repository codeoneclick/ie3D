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

class CHVertexBuffer;
class CSVertexBuffer;
class CHIndexBuffer;
class CSIndexBuffer;

class CBone final : public std::enable_shared_from_this<CBone>
{
private:
    
protected:
    
    std::string	m_name;
	i32	m_id;
	i32	m_parentId;
    
    std::shared_ptr<CBone> m_parent;
    std::vector<std::shared_ptr<CBone> > m_childs;
   
    glm::mat4x4 m_baseTransformation;
    glm::mat4x4* m_transformation;
    glm::mat4x4 m_bindTransformation;
    
public:
    
    CBone(i32 _id, i32 _parentId);
    ~CBone(void);
    
    void Set_Name(const std::string& _name)
    {
        m_name = _name;
    };
    
    std::string Get_Name(void)
    {
        return m_name;
    };
    
    void AddChild(std::shared_ptr<CBone> _bone);
    std::shared_ptr<CBone> FindChild(i32 _id);
    void Update(void);
    void Set_BindTransformation(void);
    
    inline std::vector<std::shared_ptr<CBone> >& Get_Childs(void)
    {
        return m_childs;
    };
    
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
    
	inline void Set_Transformation(glm::mat4x4* _transformation)
	{
		m_transformation = _transformation;
	};
    
	inline glm::mat4x4* Get_Transformation(void) const
	{
		return m_transformation;
	};
    
    inline glm::mat4x4 Get_BaseTransformation(void) const
    {
        return m_baseTransformation;
    };
};

#endif 



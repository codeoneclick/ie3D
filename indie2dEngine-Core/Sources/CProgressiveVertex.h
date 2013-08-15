
#ifndef CProgressiveVertex_h
#define CProgressiveVertex_h

#include "HCommon.h"

class CProgressiveTriangle;

class CProgressiveVertex
{
    
private:
    
	ui32 m_id;
    
    glm::vec3 m_position;
    glm::vec2 m_texcoord;
    glm::vec3 m_normal;
    
    bool m_isActive;
    
    std::vector<std::shared_ptr<CProgressiveTriangle> > m_faces;
    glm::mat4x4 m_quadric;
    f32 m_quadricTriangleArea;
    
protected:

public:

    
	CProgressiveVertex(ui32 _id, const glm::vec3& _position, const glm::vec2& _texcoord, const glm::vec3& _normal);

	virtual ~CProgressiveVertex();

	inline ui32 Get_Id(void) const
    {
        return m_id;
    };

	inline void Set_Id(ui32 _id)
    {
        m_id = _id;
    };

	inline const glm::vec3& Get_Position(void) const
    {
        return m_position;
    };

	inline void Set_Position(const glm::vec3& _position)
    {
        m_position = _position;
    };

    inline const glm::vec2& Get_Texcoord(void) const
    {
        return m_texcoord;
    };
    
    inline void Set_Texcoord(const glm::vec2& _texcoord)
    {
        m_texcoord = _texcoord;
    };
    
    inline const glm::vec3& Get_Normal(void) const
    {
        return m_normal;
    };
    
    inline void Set_Normal(const glm::vec3& _normal)
    {
        m_normal = _normal;
    };
    
	inline bool IsActive(void) const
    {
        return m_isActive;
    };
    
    inline void Set_Active(bool _value)
    {
        m_isActive = _value;
    };
    
    inline std::vector<std::shared_ptr<CProgressiveTriangle> >& Get_Faces(void)
    {
        return m_faces;
    };
    
    inline const glm::mat4x4& Get_Quadric(void) const
    {
        return m_quadric;
    };
    
    inline void Set_Quadric(const glm::mat4x4& _quadric)
    {
        m_quadric = _quadric;
    };
    
    inline const f32 Get_QuadricTriangleArea(void) const
    {
        return m_quadricTriangleArea;
    };
    
    inline void Set_QuadricTriangleArea(f32 _quadricTriangleArea)
    {
        m_quadricTriangleArea = _quadricTriangleArea;
    };

	void AddFace(std::shared_ptr<CProgressiveTriangle> _triangle);
	void RemoveFace(std::shared_ptr<CProgressiveTriangle> _triangle);

	void CalculateQuadric(bool _useTriangleArea);

	bool IsOrphan(void) const;
};

#endif 

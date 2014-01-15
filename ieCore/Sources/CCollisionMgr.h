//
//  CCollisionMgr.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/19/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CCollisionMgr_h
#define CCollisionMgr_h

#include "HCommon.h"
#include "IInputTapRecognizerHandler.h"

class CCamera;
class ICollisionHandler;
class CVertexBuffer;
class CIndexBuffer;

class CCollisionMgr final : public IInputTapRecognizerHandler
{
private:
    
protected:
    
    std::shared_ptr<CCamera> m_camera;
    std::set<std::shared_ptr<ICollisionHandler> > m_handlers;
    
    bool _CollisionPoint(std::shared_ptr<CVertexBuffer> _vertexBuffer, std::shared_ptr<CIndexBuffer> _indexBuffer, const glm::mat4x4& _worldMatrix, const glm::vec3& _origin, const glm::vec3& _direction, glm::vec3* _point);
    bool _TriangleIntersection(const glm::vec3& _trianglePoint_01, glm::vec3& _trianglePoint_02, glm::vec3& _trianglePoint_03, const glm::vec3& _origin, const glm::vec3& _direction, glm::vec3* _intersectPoint);
    
    void _OnInputTapRecognizerPressed(const glm::ivec2& _point);
    void _OnInputTapRecognizerMoved(const glm::ivec2& _point);
    void _OnInputTapRecognizerReleased(const glm::ivec2& _point);
    
public:
    
    CCollisionMgr(void);
    ~CCollisionMgr(void);
    
    void Set_Camera( std::shared_ptr<CCamera> _camera)
    {
        assert(_camera != NULL);
        m_camera = _camera;
    };
    
    static void Unproject(const glm::ivec2& _point, const glm::mat4x4& _viewMatrix, const glm::mat4x4& _projectionMatrix, glm::ivec4 _viewport, glm::vec3* _origin, glm::vec3* _direction);
    
    void RegisterCollisionHandler(std::shared_ptr<ICollisionHandler> _handler);
    void UnregisterCollisionHandler(std::shared_ptr<ICollisionHandler> _handler);
};

#endif 

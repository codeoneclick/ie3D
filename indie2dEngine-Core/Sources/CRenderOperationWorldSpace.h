//
//  CRenderOperationWorldSpace.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CRenderOperationWorldSpace_h
#define CRenderOperationWorldSpace_h

#include "HCommon.h"

class CTexture;
class IRenderHandler;
class CBatchingMgr;


class CRenderOperationWorldSpace final
{
private:
    
protected:
    
    std::shared_ptr<CTexture> m_operatingColorTexture;
    std::shared_ptr<CTexture> m_operatingDepthTexture;
    ui32 m_frameBufferHandle;
    ui32 m_frameWidth;
    ui32 m_frameHeight;
    glm::vec4 m_clearColor;
    std::map<ui32, std::set< std::shared_ptr<IRenderHandler> > > m_handlers;
    std::string m_mode;
    ui32 m_index;
    ui32 m_numTriangles;
    
public:
    
    CRenderOperationWorldSpace(ui32 _frameWidth, ui32 _frameHeight, const glm::vec4& _clearColor, const std::string& _mode, ui32 _index);
    ~CRenderOperationWorldSpace(void);
    
    inline std::shared_ptr<CTexture> Get_OperatingColorTexture(void) const
    {
        return m_operatingColorTexture;
    };
    
    inline std::shared_ptr<CTexture> Get_OperatingDepthTexture(void) const
    {
        return m_operatingDepthTexture;
    };
    
    inline ui32 Get_NumTriangles(void) const
    {
        return m_numTriangles;
    };
    
    inline std::string Get_Mode(void) const
    {
        return m_mode;
    };
    
    inline ui32 Get_Index(void) const
    {
        return m_index;
    };
    
    void RegisterRenderHandler(const std::shared_ptr<IRenderHandler>& _handler);
    void UnregisterRenderHandler(const std::shared_ptr<IRenderHandler>& _handler);
    
    void Batch(void);
    void Bind(void);
    void Unbind(void);
    
    void Draw(void);
};

#endif 

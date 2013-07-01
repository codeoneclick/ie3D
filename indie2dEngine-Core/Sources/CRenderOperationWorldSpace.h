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

class CRenderOperationWorldSpace final
{
private:
    
protected:
    
    std::shared_ptr<CTexture> m_operatingColorTexture;
    std::shared_ptr<CTexture> m_operatingDepthTexture;
    ui32 m_frameBufferHandle;
    //ui32 m_depthBufferHandle;
    ui32 m_frameWidth;
    ui32 m_frameHeight;
    std::map<ui32, std::set< std::shared_ptr<IRenderHandler> > > m_handlers;
    std::string m_mode;
    
public:
    
    CRenderOperationWorldSpace(ui32 _frameWidth, ui32 _frameHeight, const std::string& _mode);
    ~CRenderOperationWorldSpace(void);
    
    inline std::shared_ptr<CTexture> Get_OperatingColorTexture(void)
    {
        return m_operatingColorTexture;
    };
    
    inline std::shared_ptr<CTexture> Get_OperatingDepthTexture(void)
    {
        return m_operatingDepthTexture;
    };
    
    void RegisterRenderHandler(std::shared_ptr<IRenderHandler> _handler);
    void UnregisterRenderHandler(std::shared_ptr<IRenderHandler> _handler);
    
    void Bind(void);
    void Unbind(void);
    
    void Draw(void);
};

#endif 

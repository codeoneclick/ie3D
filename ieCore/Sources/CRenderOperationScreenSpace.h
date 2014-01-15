//
//  CRenderOperationScreenSpace.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CRenderOperationScreenSpace_h
#define CRenderOperationScreenSpace_h

#include "HCommon.h"

class CTexture;
class CMaterial;
class CQuad;

class CRenderOperationScreenSpace final
{
private:
    
protected:
    
    std::shared_ptr<CTexture> m_operatingTexture;
    std::shared_ptr<CMaterial> m_material;
    ui32 m_frameBufferHandle;
    ui32 m_depthBufferHandle;
    ui32 m_frameWidth;
    ui32 m_frameHeight;
    std::shared_ptr<CQuad> m_quad;
    std::string m_mode;
    
public:
    
    CRenderOperationScreenSpace(ui32 _frameWidth, ui32 _frameHeight, const std::string& _mode, std::shared_ptr<CMaterial> _material);
    ~CRenderOperationScreenSpace(void);
    
    inline std::shared_ptr<CTexture> Get_OperatingTexture(void)
    {
        return m_operatingTexture;
    };
    
    inline std::shared_ptr<CMaterial> Get_Material(void)
    {
        return m_material;
    };
    
    void Bind(void);
    void Unbind(void);
    
    void Draw(void);
};

#endif 

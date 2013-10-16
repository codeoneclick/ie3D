//
//  CRenderOperationOutput.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CRenderOperationOutput_h
#define CRenderOperationOutput_h

#include "HCommon.h"

class CMaterial;
class CQuad;

class CRenderOperationOutput
{
private:
    
protected:
    
    std::shared_ptr<CMaterial> m_material;
    ui32 m_frameBufferHandle;
    ui32 m_renderBufferHandle;
    ui32 m_frameWidth;
    ui32 m_frameHeight;
    std::shared_ptr<CQuad> m_quad;
    
public:
    
    CRenderOperationOutput(ui32 _frameWidth, ui32 _frameHeight, std::shared_ptr<CMaterial> _material, ui32 _frameBufferHandle, ui32 _renderBufferHandle);
    ~CRenderOperationOutput(void);
    
    inline std::shared_ptr<CMaterial> Get_Material(void)
    {
        return m_material;
    };
    
    void Bind(void);
    void Unbind(void);
    void Draw(void);
};



#endif 

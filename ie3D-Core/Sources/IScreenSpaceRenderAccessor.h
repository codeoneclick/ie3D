//
//  IScreenSpaceRenderAccessor.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IScreenSpaceRenderAccessor_h
#define IScreenSpaceRenderAccessor_h

#include "HCommon.h"
#include "HDeclaration.h"

class IScreenSpaceRenderAccessor
{
private:

protected:

    std::unordered_map<std::string, CSharedTexture> m_customTextures;
    
public:
    
    IScreenSpaceRenderAccessor(void);
    virtual ~IScreenSpaceRenderAccessor(void);
    
	virtual CSharedTexture getSSOperationTexture(const std::string& mode) = 0;
    virtual CSharedMaterial getSSOperationMaterial(const std::string& mode) = 0;
    virtual CSharedTexture preprocessSSOperationTexture(CSharedMaterialRef material,
                                                        ui32 width,
                                                        ui32 height) = 0;
    
    void addCustomTexture(CSharedTextureRef texture);
    CSharedTexture getCustomTexture(const std::string& guid);
};

#endif

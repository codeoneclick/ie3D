//
//  IScreenSpaceTextureAccessor.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IScreenSpaceTextureAccessor_h
#define IScreenSpaceTextureAccessor_h

#include "HCommon.h"
#include "HDeclaration.h"

class IScreenSpaceTextureAccessor
{
private:

protected:

    std::unordered_map<std::string, CSharedTexture> m_customTextures;
    
public:
    
    IScreenSpaceTextureAccessor(void);
    virtual ~IScreenSpaceTextureAccessor(void);
    
	virtual CSharedTexture Get_RenderOperationTexture(const std::string& mode) = 0;
    
    virtual CSharedTexture Get_CustomScreenSpaceOperationTexture(CSharedMaterialRef material,
                                                                 ui32 width,
                                                                 ui32 height) = 0;
    
    void addCustomTexture(CSharedTextureRef texture);
    CSharedTexture getCustomTexture(const std::string& guid);
};

#endif

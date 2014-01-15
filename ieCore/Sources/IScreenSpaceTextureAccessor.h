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

class CTexture;
class CMaterial;

class IScreenSpaceTextureAccessor
{
private:

protected:

public:
    
    IScreenSpaceTextureAccessor(void);
    virtual ~IScreenSpaceTextureAccessor(void);
    
	virtual std::shared_ptr<CTexture> Get_RenderOperationTexture(const std::string& _mode) = 0;
    virtual std::shared_ptr<CTexture> Get_CustomScreenSpaceOperationTexture(const std::shared_ptr<CMaterial> _material, ui32 _textureWidth, ui32 _textureHeight) = 0;
};

#endif

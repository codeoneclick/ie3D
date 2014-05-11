//
//  CCubemapTexture.h
//  ie3D-Core
//
//  Created by Sergey Sergeev on 5/6/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CCubemapTexture_h
#define CCubemapTexture_h

#include "CTexture.h"

class CCubemapTexture : public CTexture, public IResourceLoadingHandler
{
private:
    
    CSharedTexture m_xpositive;
    CSharedTexture m_xnegative;
    CSharedTexture m_ypositive;
    CSharedTexture m_ynegative;
    CSharedTexture m_zpositive;
    CSharedTexture m_znegative;
    
protected:
    
    virtual void onResourceLoaded(ISharedResourceRef resource, bool success);
    
public:
    
    CCubemapTexture(CSharedTextureRef xpositive, CSharedTextureRef xnegative,
                    CSharedTextureRef ypositive, CSharedTextureRef ynegative,
                    CSharedTextureRef zpositive, CSharedTextureRef znegative);
    
    static CSharedCubemapTexture constructCustomCubemapTexture(CSharedTextureRef xpositive, CSharedTextureRef xnegative,
                                                               CSharedTextureRef ypositive, CSharedTextureRef ynegative,
                                                               CSharedTextureRef zpositive, CSharedTextureRef znegative);
    ~CCubemapTexture(void);
    
    void bind(void) const;
    void unbind(void) const;
};

#endif

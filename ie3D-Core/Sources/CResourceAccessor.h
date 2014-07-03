//
//  CResourceAccessor.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CResourceAccessor_h
#define CResourceAccessor_h

#include "HDeclaration.h"
#include "HEnums.h"

class CResourceAccessor
{
private:
    
protected:
    
    CSharedResourceLoader m_resourceLoader;
    std::unordered_map<std::string, CSharedTexture> m_customTextures;
    
#if defined(__NDK__)
    
    struct AAssetManager* m_assetManager;

#endif
    
public:
    
#if defined(__NDK__)
    
    CResourceAccessor(const AAssetManager* assetManager);
    
#endif
    
    CResourceAccessor(void);
    ~CResourceAccessor(void);
    
    CSharedResourceLoader getResourceLoader(void) const;
    
    void addCustomTexture(const std::string& textureName, CSharedTextureRef texture);
    CSharedTexture getTexture(const std::string& filename) const;
    
    CSharedShader getShader(const std::string& vsFilename,
                            const std::string& fsFilename) const;
    
    CSharedMesh getMesh(const std::string& filename) const;
};

#endif 

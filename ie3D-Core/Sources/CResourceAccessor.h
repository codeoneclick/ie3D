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
    
    void addCustomTexture(const std::string& guid, CSharedTextureRef texture);
    CSharedTexture getCustomTexture(const std::string& guid, bool sync = false) const;
    CSharedTexture getTexture(const std::string& filename, bool sync = false) const;
    
    CSharedShader getShader(const std::string& vsFilename,
                            const std::string& fsFilename) const;
    
    CSharedMesh getMesh(const std::string& filename) const;
    
    CSharedAnimationSequence getAnimationSequence(const std::string& filename) const;
    
    void removeLoadingDependecies(ISharedResourceLoadingHandler handler);
};

#endif 

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
    
    std::array<ISharedResourceLoader, E_RESOURCE_LOADER_MAX> m_resourceLoaders;
    
#if defined(__NDK__)
    
    struct AAssetManager* m_assetManager;

#endif
    
public:
    
#if defined(__NDK__)
    
    CResourceAccessor(const AAssetManager* assetManager);
    
#endif
    
    CResourceAccessor(void);
    ~CResourceAccessor(void);
    
    CSharedTexture getTexture(const std::string& filename);
    CSharedShader getShader(const std::string& vsFilename, const std::string& fsFilename);
    CSharedMesh getMesh(const std::string& filename);
};

#endif 

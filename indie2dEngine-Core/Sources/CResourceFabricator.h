//
//  CResourceFabricator.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CResourceFabricator_h
#define CResourceFabricator_h

#include "HCommon.h"

class CTexture;
class CShader;
class CTextureLoader;
class CShaderLoader;

class CResourceFabricator
{
private:
    
protected:
    
    std::shared_ptr<CTextureLoader> m_textureLoader;
    std::shared_ptr<CShaderLoader> m_shaderLoader;
    
public:
    
    CResourceFabricator(void);
    ~CResourceFabricator(void);
    
    std::shared_ptr<CTexture> CreateTexture(const std::string& _filename);
    std::shared_ptr<CShader> CreateShader(const std::string& _vsFilename, const std::string& _fsFilename);
    
};

#endif 

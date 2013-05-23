//
//  CTextureLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/22/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CTextureLoadingOperation_h
#define CTextureLoadingOperation_h

#include "HCommon.h"
#include "IResourceLoadingOperation.h"

class IResource;

class CTextureLoadingOperation final : public IResourceLoadingOperation
{
private:
    
protected:
    
    std::string m_filename;
    
public:
    
    CTextureLoadingOperation(const std::string& _filename, std::shared_ptr<IResource> _resource);
    ~CTextureLoadingOperation(void);
    
    void Serialize(void);
    void Commit(void);
    
};
#endif 

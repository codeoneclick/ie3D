//
//  CMeshLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CMeshLoadingOperation_h
#define CMeshLoadingOperation_h

#include "HCommon.h"
#include "IResourceLoadingOperation.h"

class IResource;

class CMeshLoadingOperation final : public IResourceLoadingOperation
{
private:
    
protected:
    
    std::string m_filename;
    
public:
    
    CMeshLoadingOperation(const std::string& _filename, std::shared_ptr<IResource> _resource);
    ~CMeshLoadingOperation(void);
    
    void Serialize(void);
    void Commit(void);
    
};

#endif 

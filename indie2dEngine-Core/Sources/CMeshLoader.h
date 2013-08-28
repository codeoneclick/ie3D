//
//  CMeshLoader.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CMeshLoader_h
#define CMeshLoader_h

#include "IResourceLoader.h"

class CMesh;
class IResourceLoadingHandler;
class CMeshLoader final : public IResourceLoader
{
private:
    
protected:
    
public:
    
    CMeshLoader(void);
    ~CMeshLoader(void);
    
    std::shared_ptr<CMesh> StartLoadOperation(const std::string& _filename);
};

#endif

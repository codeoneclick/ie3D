//
//  CMeshLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CMeshLoadingOperation_h
#define CMeshLoadingOperation_h

#include "IResourceLoadingOperation.h"

class IResource;

class CMeshLoadingOperation final : public IResourceLoadingOperation
{
private:
    
protected:
    
    std::string m_meshFilename;
    std::string m_skeletonFilename;
    std::string m_sequenceFilename;
    
public:
    
    CMeshLoadingOperation(const std::string& meshFilename,
                          const std::string& skeletonFilename,
                          const std::string& sequenceFilename,
                          ISharedResourceRef resource);
    ~CMeshLoadingOperation(void);
    
    void serialize(void);
    void commit(void);
    
};

#endif 

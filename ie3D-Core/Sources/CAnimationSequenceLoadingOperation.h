//
//  CAnimationSequenceLoadingOperation.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 11/13/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CAnimationSequenceLoadingOperation_h
#define CAnimationSequenceLoadingOperation_h

#include "IResourceLoadingOperation.h"

class IResource;

class CAnimationSequenceLoadingOperation final : public IResourceLoadingOperation
{
private:
    
protected:
    
    std::string m_filename;
    
public:
    
    CAnimationSequenceLoadingOperation(const std::string& filename,
                                       ISharedResourceRef resource);
    ~CAnimationSequenceLoadingOperation(void);
    
    void serialize(void);
    void commit(void);
};

#endif

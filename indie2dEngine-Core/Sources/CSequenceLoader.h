//
//  CSequenceLoader.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CSequenceLoader_h
#define CSequenceLoader_h

#include "IResourceLoader.h"

class CSequence;
class IResourceLoadingHandler;
class CSequenceLoader final : public IResourceLoader
{
private:
    
protected:
    
public:
    
    CSequenceLoader(void);
    ~CSequenceLoader(void);
    
    std::shared_ptr<CSequence> StartLoadOperation(const std::string& _filename);
};

#endif

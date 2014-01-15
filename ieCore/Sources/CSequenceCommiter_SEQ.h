//
//  CSequenceCommiter_SEQ.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CSequenceCommiter_SEQ_h
#define CSequenceCommiter_SEQ_h

#include "IResourceCommiter.h"

class CSequenceCommiter_SEQ final : public IResourceCommiter
{
private:
    
protected:
    
public:
    
    CSequenceCommiter_SEQ(const std::string& _guid, std::shared_ptr<IResource> _resource);
    ~CSequenceCommiter_SEQ(void);
    
    void Commit(void);
};


#endif

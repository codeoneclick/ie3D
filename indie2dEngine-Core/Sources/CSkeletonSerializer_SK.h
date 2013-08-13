//
//  CSkeletonSerializer_SK.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CSkeletonSerializer_SK_h
#define CSkeletonSerializer_SK_h

#include "IResourceSerializer.h"

class CSkeletonSerializer_SK final : public IResourceSerializer
{
private:
    
protected:
    
    std::string m_filename;
    
public:
    
    CSkeletonSerializer_SK(const std::string& _filename, std::shared_ptr<IResource> _resource);
    ~CSkeletonSerializer_SK(void);
    
    void Serialize(void);
};

#endif

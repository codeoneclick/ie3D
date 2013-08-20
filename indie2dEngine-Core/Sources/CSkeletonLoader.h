//
//  CSkeletonLoader.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CSkeletonLoader_h
#define CSkeletonLoader_h

#include "IResourceLoader.h"

class CSkeleton;
class IResourceLoadingHandler;
class CSkeletonLoader final : public IResourceLoader
{
private:
    
protected:
    
public:
    
    CSkeletonLoader(void);
    ~CSkeletonLoader(void);
    
    std::shared_ptr<CSkeleton> StartLoadOperation(const std::string& _filename, std::shared_ptr<IResourceLoadingHandler> _handler);
};

#endif 

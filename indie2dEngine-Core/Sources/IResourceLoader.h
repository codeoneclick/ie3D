//
//  IResourceLoader.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IResourceLoader_h
#define IResourceLoader_h

#include "HCommon.h"

class IResourceLoadingOperation;
class IResource;

class IResourceLoader
{
private:
    
protected:
    
    std::map<std::string, std::shared_ptr<IResourceLoadingOperation> > m_operationsQueue;
    std::map<std::string, std::shared_future< std::shared_ptr<IResource> > > m_resourceContainer;
    
    std::mutex m_mutex;
    std::thread m_thread;
    std::atomic_int m_atomic;
    
    virtual void _Update(void);
    virtual void _Thread(void);
    
public:
    
    IResourceLoader(void);
    virtual ~IResourceLoader(void);
    
	void UnloadResource(std::shared_future<std::shared_ptr<IResource> > _resource);
};

#endif 

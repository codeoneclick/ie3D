//
//  CThreadOperationPool.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 8/19/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CThreadOperationPool_h
#define CThreadOperationPool_h

#include "HCommon.h"
#include "HDeclaration.h"
#include "HEnums.h"

class CThreadOperationPool
{
private:
    
protected:
    
    static CThreadOperationPool* m_sharedInstance;
    
    std::array<std::queue<CSharedThreadOperation>, E_THREAD_OPERATION_QUEUE_MAX> m_operations;
    
    ui8 m_isRunning;
    std::thread m_thread;
    std::mutex m_mutex;
    
    CSharedThreadOperation nextOperation(E_THREAD_OPERATION_QUEUE operationQueue);
    void popOperation(E_THREAD_OPERATION_QUEUE operationQueue);
    
    void updateThread(void);
    
public:
    
    CThreadOperationPool(void);
    ~CThreadOperationPool(void);
    
    static CThreadOperationPool* sharedInstance(void);
    
    void addOperation(CSharedThreadOperationRef operation, E_THREAD_OPERATION_QUEUE operationQueue);
    
    void update(void);
};

#endif

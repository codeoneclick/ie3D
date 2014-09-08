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
    
    std::array<std::array<std::queue<CSharedThreadOperation>, E_THREAD_OPERATION_QUEUE_MAX>, 5> m_operations;
    
    ui8 m_isRunning;
    std::array<std::thread, 5> m_threads;
    std::mutex m_mutex;
    std::set<CSharedThreadOperation> m_uniqueOperations;
    
    CSharedThreadOperation nextOperation(ui32 threadId, E_THREAD_OPERATION_QUEUE operationQueue);
    void popOperation(ui32 threadId, E_THREAD_OPERATION_QUEUE operationQueue);
    bool isQueueEmpty(ui32 threadId, E_THREAD_OPERATION_QUEUE operationQueue);
    
    void updateThread(ui32 threadId);
    
    void updateThread_01(void);
    void updateThread_02(void);
    void updateThread_03(void);
    void updateThread_04(void);
    void updateThread_05(void);
    
public:
    
    CThreadOperationPool(void);
    ~CThreadOperationPool(void);
    
    static CThreadOperationPool* sharedInstance(void);
    
    void addOperation(CSharedThreadOperationRef operation, E_THREAD_OPERATION_QUEUE operationQueue);
    
    void update(void);
};

#endif

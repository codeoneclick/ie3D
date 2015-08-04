//
//  CThreadOperation.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 8/19/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CThreadOperation_h
#define CThreadOperation_h

#include "HCommon.h"
#include "HDeclaration.h"
#include "HEnums.h"

class CThreadOperation : public std::enable_shared_from_this<CThreadOperation>
{
private:
    
protected:

    E_THREAD_OPERATION_QUEUE m_operationQueue;
    
    std::deque<CSharedThreadOperation> m_dependecies;
    
    std::function<void(void)> m_executionBlock;
    std::function<void(void)> m_cancelBlock;
    
    std::recursive_mutex m_mutex;
    ui8 m_status;
    
public:
    
    CThreadOperation(E_THREAD_OPERATION_QUEUE operationQueue);
    ~CThreadOperation(void);
    
    void setExecutionBlock(std::function<void(void)> callback);
    void setCancelBlock(std::function<void(void)> callback);
    
    void addDependency(CSharedThreadOperationRef operation);
    void execute(void);
    void addToExecutionQueue(void);
    void cancel(void);
    
    bool isExecuted(void);
    bool isCanceled(void);
    bool isCompleted(void);
    
    E_THREAD_OPERATION_QUEUE getOperationQueueName(void);
    CSharedThreadOperation nextOperation(void);
    bool popOperation(void);
    bool isQueueEmpty(void);
};

#endif

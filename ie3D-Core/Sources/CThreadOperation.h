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
    
    friend class CThreadOperationPool;
    
    E_THREAD_OPERATION_QUEUE m_operationQueue;
    
    std::queue<CSharedThreadOperation> m_dependecies;
    
    std::function<void(void)> m_executionBlock;
    std::function<void(void)> m_cancelBlock;
    
    bool m_isDone;
    bool m_isCanceled;
    
    CSharedThreadOperation nextOperation(void);
    bool popOperation(void);
    
public:
    
    CThreadOperation(E_THREAD_OPERATION_QUEUE operationQueue);
    ~CThreadOperation(void);
    
    void setExecutionBlock(std::function<void(void)> callback);
    void setCancelBlock(std::function<void(void)> callback);
    
    void addDependency(CSharedThreadOperationRef operation);
    void execute(void);
    void cancel(void);
    
    bool getIsDone(void);
    bool getIsCanceled(void);
};

#endif

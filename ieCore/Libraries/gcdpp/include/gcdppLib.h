//
//  gcdppLib.h
//  gcdpp
//
//  Created by Sergey Sergeev on 6/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef gcdppLib_h
#define gcdppLib_h

#include "gcdppCommon.h"
#include "gcdppQueue_t.h"
#include "gcdppMainQueue_t.h"
#include "gcdppTask_t.h"

namespace gcdpp
{
    namespace queue
    {
        enum GCDPP_DISPATCH_QUEUE_PRIORITY
        {
            GCDPP_DISPATCH_QUEUE_PRIORITY_LOW = 1,
            GCDPP_DISPATCH_QUEUE_PRIORITY_DEFAULT,
            GCDPP_DISPATCH_QUEUE_PRIORITY_HIGH,
            GCDPP_DISPATCH_QUEUE_PRIORITY_MAIN,
            GCDPP_DISPATCH_QUEUE_PRIORITY_MAX
        };
        
        std::shared_ptr<gcdppQueue_t> GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY _priority);
        std::shared_ptr<gcdppMainQueue_t> GetMainQueue(void);
    };
    
    namespace impl
    {
        extern void UpdateMainQueue(void);
        
        template<class... ARGS>
        extern size_t DispatchAsync(std::shared_ptr<gcdppQueue_i> _queue, std::function<void(ARGS... args)> _function, ARGS... args);
        
        template<class... ARGS>
        extern size_t DispatchAsyncAfter(std::shared_ptr<gcdppQueue_i> _queue, std::function<void(ARGS... args)> _function, std::chrono::milliseconds _interval, ARGS... args);
        
        extern void DispatchCancel(size_t _id);
    };

    class gcdppImpl
    {
    private:
        
    protected:
        
        friend std::shared_ptr<gcdppQueue_t> queue::GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY _priority);
        friend std::shared_ptr<gcdppMainQueue_t> queue::GetMainQueue(void);
        
        
        friend void impl::UpdateMainQueue(void);
        
        template<class... ARGS>
        friend size_t impl::DispatchAsync(std::shared_ptr<gcdppQueue_i> _queue, std::function<void(ARGS... args)> _function, ARGS... args);
        
        template<class... ARGS>
        friend size_t impl::DispatchAsyncAfter(std::shared_ptr<gcdppQueue_i> _queue, std::function<void(ARGS... args)> _function, std::chrono::milliseconds _interval, ARGS... args);
        
        friend void impl::DispatchCancel(size_t _id);
        
        std::shared_ptr<gcdppQueue_i> m_poolQueue[gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY_MAX];
        
        static std::shared_ptr<gcdppImpl> Instance(void);
        
        std::shared_ptr<gcdppQueue_t> GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY _priority);
        std::shared_ptr<gcdppMainQueue_t> GetMainQueue(void);
        
        template<class... ARGS>
        void DispatchAsync(std::shared_ptr<gcdppQueue_i> _queue, std::function<void(ARGS... args)> _function, ARGS... args);
        
    public:
        
        gcdppImpl(void);
        ~gcdppImpl(void);
        
    };
    
    template<class... ARGS>
    void gcdppImpl::DispatchAsync(std::shared_ptr<gcdppQueue_i> _queue, std::function<void(ARGS... args)> _function, ARGS... args)
    {
        std::shared_ptr<gcdppTask_t<std::function<void(ARGS...)>, ARGS... > > task = std::make_shared<gcdppTask_t<std::function<void(ARGS...)>, ARGS... > >(_function, std::forward<ARGS>(args)...);
        _queue->AppendTask(task);
    };
    

    extern inline std::shared_ptr<gcdppQueue_t> queue::GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY _priority)
    {
        return gcdppImpl::Instance()->GetGlobalQueue(_priority);
    };
        
    extern inline std::shared_ptr<gcdppMainQueue_t> queue::GetMainQueue(void)
    {
        return gcdppImpl::Instance()->GetMainQueue();
    };
    
    extern inline void impl::UpdateMainQueue(void)
    {
        assert(gcdppImpl::Instance()->GetMainQueue() != nullptr);
        gcdppImpl::Instance()->GetMainQueue()->_Update();
    };
    
    template<class... ARGS>
    extern inline size_t impl::DispatchAsync(std::shared_ptr<gcdppQueue_i> _queue, std::function<void(ARGS... args)> _function, ARGS... args)
    {
        gcdppImpl::Instance()->DispatchAsync(_queue, _function, std::forward<ARGS>(args)...);
        return 0;
    };
        
    template<class... ARGS>
    extern inline size_t impl::DispatchAsyncAfter(std::shared_ptr<gcdppQueue_i> _queue, std::function<void(ARGS... args)> _function, std::chrono::milliseconds _interval, ARGS... args)
    {
        return 0;
    };
        
    extern inline void impl::DispatchCancel(size_t _id)
    {
        
    };
};


#endif

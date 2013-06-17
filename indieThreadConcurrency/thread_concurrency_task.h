//
//  thread_concurrency_task.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef thread_concurrency_task_h
#define thread_concurrency_task_h

#include "thread_concurrency_common.h"

template<unsigned int NUM>
struct apply_
{
    template<typename... F_ARGS, typename... T_ARGS, typename... ARGS>
    static void apply(std::function<void(F_ARGS... args)> _function, std::tuple<T_ARGS...> const& _targs,
                      ARGS... args)
    {
        apply_<NUM-1>::apply(_function, _targs, std::get<NUM-1>(_targs), args...);
    }
};

template<>
struct apply_<0>
{
    template<typename... F_ARGS, typename... T_ARGS, typename... ARGS>
    static void apply(std::function<void(F_ARGS... args)> _function, std::tuple<T_ARGS...> const&,
                      ARGS... args)
    {
        _function(args...);
    }
};

template<typename... F_ARGS, typename... T_ARGS>
void apply(std::function<void(F_ARGS... _fargs)> _function, std::tuple<T_ARGS...> const& _targs)
{
    apply_<sizeof...(T_ARGS)>::apply(_function, _targs);
}

class i_thread_concurrency_task
{
private:
    
protected:
    
public:
    
    i_thread_concurrency_task(void)
    {
        std::cout<<"[i_thread_concurrency_task created]"<<std::endl;
    };
    
    virtual ~i_thread_concurrency_task(void)
    {
        std::cout<<"[i_thread_concurrency_task deleted]"<<std::endl;
    };
    
    virtual void execute(void)
    {
        assert(false);
    };
};

template<class FUCTION, class... ARGS> class thread_concurrency_task : public i_thread_concurrency_task
{
protected:
    
    FUCTION m_function;
    std::tuple<ARGS...> m_args;
    
public:
    thread_concurrency_task(FUCTION _function, ARGS... _args)
    {
        m_function = _function;
        m_args = std::make_tuple(_args...);
        std::cout<<"[thread_concurrency_task created]"<<std::endl;
    };
    
    ~thread_concurrency_task(void)
    {
        std::cout<<"[~thread_concurrency_task deleted]"<<std::endl;
    };
    
    void execute(void)
    {
        apply(m_function, m_args);
        std::thread::id this_id = std::this_thread::get_id();
        std::cout << "[execute] thread : "<<this_id<<std::endl;
    };
};


#endif

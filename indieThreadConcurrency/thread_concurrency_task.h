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

// Recursive case, unpack Nth argument
template<unsigned int N>
struct Apply_aux
{
    template<typename... ArgsF, typename... ArgsT, typename... Args>
    static void apply(std::function<void(ArgsF... args)> f, std::tuple<ArgsT...> const& t,
                      Args... args)
    {
        Apply_aux<N-1>::apply(f, t, std::get<N-1>(t), args...);
    }
};

// Terminal case, call the function with unpacked arguments
template<>
struct Apply_aux<0>
{
    template<typename... ArgsF, typename... ArgsT, typename... Args>
    static void apply(std::function<void(ArgsF... args)> f, std::tuple<ArgsT...> const&,
                      Args... args)
    {
        f(args...);
    }
};

// Actual apply function
template<typename... ArgsF, typename... ArgsT>
void apply(std::function<void(ArgsF... args)> f, std::tuple<ArgsT...> const& t)
{
    Apply_aux<sizeof...(ArgsT)>::apply(f, t);
}

void f(int p1, double p2, std::string p3);
void g(int p1, std::string p2);


int foooooo();


/**
 * Object Function Tuple Argument Unpacking
 *
 * This recursive template unpacks the tuple parameters into
 * variadic template arguments until we reach the count of 0 where the function
 * is called with the correct parameters
 *
 * @tparam N Number of tuple arguments to unroll
 *
 * @ingroup g_util_tuple
 */
template < uint N >
struct apply_obj_func
{
    template <typename... TARGS, typename... ARGS>
    static void applyTuple(std::function<void(ARGS... args)> _function, const std::tuple<TARGS...>& _targs, ARGS... _args)
    {
        apply_obj_func<N-1>::applyTuple(_function, _targs, std::get<N-1>(_targs));
    }
};

//-----------------------------------------------------------------------------

/**
 * Object Function Tuple Argument Unpacking End Point
 *
 * This recursive template unpacks the tuple parameters into
 * variadic template arguments until we reach the count of 0 where the function
 * is called with the correct parameters
 *
 * @ingroup g_util_tuple
 */
template <>
struct apply_obj_func<0>
{
    template <typename... TARGS, typename... ARGS>
    static void applyTuple(std::function<void(ARGS... args)> _function, const std::tuple<TARGS...>& _targs, ARGS... _args)
    {
        _function(std::forward<ARGS>(_args)...);
    }
};

//-----------------------------------------------------------------------------

/**
 * Object Function Call Forwarding Using Tuple Pack Parameters
 */
// Actual apply function
template <typename... ARGS, typename... TARGS>
void applyTuple(std::function<void(ARGS... args)> _function, const std::tuple<TARGS...>& _targs)
{
    foooooo();
    //apply_obj_func<sizeof...(TARGS)>::applyTuple(_function, _targs);
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
        //applyTuple(m_function, m_args);
        //m_function(m_args);
        apply(m_function, m_args);
        std::thread::id this_id = std::this_thread::get_id();
        std::cout << "[execute] thread : "<<this_id<<std::endl;
    };
};


#endif

//
//  gcdppTask_t.h
//  gcdpp
//
//  Created by Sergey Sergeev on 6/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef gcdppTask_t_h
#define gcdppTask_t_h

#include "gcdppTask_i.h"

namespace gcdpp
{
    template<unsigned int NUM>
    struct gcdppParams_t
    {
        template<typename... F_ARGS, typename... T_ARGS, typename... ARGS>
        static void Forward(std::function<void(F_ARGS... args)> _function, std::tuple<T_ARGS...> const& _targs,
                          ARGS... args)
        {
            gcdppParams_t<NUM-1>::Forward(_function, _targs, std::get<NUM-1>(_targs), args...);
        }
    };
    
    template<>
    struct gcdppParams_t<0>
    {
        template<typename... F_ARGS, typename... T_ARGS, typename... ARGS>
        static void Forward(std::function<void(F_ARGS... args)> _function, std::tuple<T_ARGS...> const&,
                          ARGS... args)
        {
            _function(args...);
        }
    };
    
    template<typename... F_ARGS, typename... T_ARGS>
    void Forward(std::function<void(F_ARGS... _fargs)> _function, std::tuple<T_ARGS...> const& _targs)
    {
        gcdppParams_t<sizeof...(T_ARGS)>::Forward(_function, _targs);
    }
    
    template<class FUCTION, class... ARGS>
    class gcdppTask_t : public gcdppTask_i
    {
    protected:
        
        FUCTION m_function;
        std::tuple<ARGS...> m_args;
        
    public:
        
        gcdppTask_t(FUCTION _function, ARGS... _args)
        {
            m_function = _function;
            m_args = std::make_tuple(_args...);
        };
        
        ~gcdppTask_t(void)
        {
            
        };
        
        void Execute(void)
        {
            assert(m_function != nullptr);
            Forward(m_function, m_args);
        };
    };
};

#endif

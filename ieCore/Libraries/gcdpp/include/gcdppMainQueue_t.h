//
//  gcdppMainQueue_t.h
//  gcdpp
//
//  Created by Sergey Sergeev on 6/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef gcdppMainQueue_t_h
#define gcdppMainQueue_t_h

#include "gcdppQueue_i.h"

namespace gcdpp
{
    namespace impl
    {
        extern inline void UpdateMainQueue(void);
    };
    
    class gcdppMainQueue_t : public gcdppQueue_i
    {
    private:
        
    protected:
 
        friend void impl::UpdateMainQueue(void);
        void _Update(void);
        
    public:
        
        gcdppMainQueue_t(const std::string& _guid);
        ~gcdppMainQueue_t(void);
        
        virtual void AppendTask(std::shared_ptr<gcdppTask_i> _task);
    };
};


#endif

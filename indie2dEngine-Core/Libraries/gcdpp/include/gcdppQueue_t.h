//
//  gcdppQueue_t.h
//  gcdpp
//
//  Created by Sergey Sergeev on 6/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef gcdppQueue_t_h
#define gcdppQueue_t_h

#include "gcdppQueue_i.h"

namespace gcdpp
{
    class gcdppQueue_t : public gcdppQueue_i
    {
    private:
        
    protected:
        
        std::mutex m_mutex;
        std::thread m_thread;
        bool m_running;
        
        void _Thread(void);
        
    public:
        
        gcdppQueue_t(const std::string& _guid);
        ~gcdppQueue_t(void);
        
        void AppendTask(std::shared_ptr<gcdppTask_i> _task);
    };
};

#endif

//
//  gcdppQueue_i.h
//  gcdpp
//
//  Created by Sergey Sergeev on 6/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef gcdppQueue_i_h
#define gcdppQueue_i_h

#include "gcdppTask_i.h"

namespace gcdpp
{
    class gcdppQueue_i
    {
    private:
        
    protected:
        
        std::string m_guid;
        std::queue<std::shared_ptr<gcdppTask_i> > m_queue;
        
    public:
        
        gcdppQueue_i(const std::string& _guid);
        virtual ~gcdppQueue_i(void);
        
        virtual void AppendTask(std::shared_ptr<gcdppTask_i> _task) = 0;
    };
};

#endif

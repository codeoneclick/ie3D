//
//  gcdppTask_i.h
//  gcdpp
//
//  Created by Sergey Sergeev on 6/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef gcdppTask_i_h
#define gcdppTask_i_h

#include "gcdppCommon.h"

namespace gcdpp
{
    class gcdppTask_i
    {
    private:
        
    protected:
        
    public:
        
        gcdppTask_i(void);
        virtual ~gcdppTask_i(void);
        
        virtual void Execute(void) = 0;
    };
};

#endif

//
//  CMmap.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/13/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CMmap_h
#define CMmap_h

#include "HCommon.h"

class CMmap
{
private:
    
    static i32 g_filedescriptors;
    i32 m_filedescriptor;
    std::string m_filename;
    void* m_pointer;
    
protected:
    
public:
    
    CMmap(void);
    ~CMmap(void);

    void* allocate(const std::string& filename);
    void deallocate(void);
    void reallocate(void);
    
    inline void* pointer(void) const { return m_pointer; };
};

#endif

//
//  CHeightmapAccessor.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CHeightmapAccessor_h
#define CHeightmapAccessor_h

#include "HCommon.h"

class CHeightmapContainer;

class CHeightmapAccessor
{
private:
    
    std::shared_ptr<CHeightmapContainer> m_container;
    
protected:
    
public:
    
    CHeightmapAccessor(void);
    ~CHeightmapAccessor(void);
    
    void create(const std::string& filename, const std::function<void(void)>& callback);
};

#endif

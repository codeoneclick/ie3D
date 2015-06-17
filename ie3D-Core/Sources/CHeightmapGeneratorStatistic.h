//
//  CHeightmapGeneratorStatistic.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 6/17/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CHeightmapGeneratorStatistic_h
#define CHeightmapGeneratorStatistic_h

#include "HCommon.h"
#include "HEnums.h"

class CHeightmapGeneratorStatistic
{
public:
    
    typedef std::function<void(const std::string& operationName, E_HEIGHTMAP_GENERATION_STATUS status, const std::string& message)> T_STATISTIC_CALLBACK;
    
private:
    
    T_STATISTIC_CALLBACK m_callback;
    
protected:
    
public:
    
    CHeightmapGeneratorStatistic(void);
    ~CHeightmapGeneratorStatistic(void);
    
    void setCallback(const T_STATISTIC_CALLBACK& callback);
    T_STATISTIC_CALLBACK getCallback(void) const;
    
    void update(const std::string& operationName, E_HEIGHTMAP_GENERATION_STATUS status, const std::string& message = "");
};

#endif

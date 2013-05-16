//
//  CGame.h
//  indi2dEngine
//
//  Created by Sergey Sergeev on 5/14/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CGame_h
#define CGame_h

#include "HCommon.h"

class CGame
{
private:
    
protected:
    
    std::string _Caller(const std::string& _value);
    
public:
    CGame(void);
    ~CGame(void);
    
    void Call_Test_01(void);
};

#endif 

//
//  CGame.cpp
//  indi2dEngine
//
//  Created by Sergey Sergeev on 5/14/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGame.h"

CGame::CGame(void)
{
    
}

CGame::~CGame(void)
{
    
}

std::string CGame::_Caller(const std::string &_value)
{
    std::cout<<"[_Caller]"<<std::endl;
    std::string value = "one, ";
    value.append(_value);
    std::cout<<value<<std::endl;
    return value;
}

void CGame::Call_Test_01(void)
{
    std::future<std::string> caller(std::async(&CGame::_Caller, this, "zero"));
    std::chrono::milliseconds duration(2000);
    std::this_thread::sleep_for(duration);
    std::cout<<"sleep"<<std::endl;
    std::string result = caller.get();
    std::cout<<result<<std::endl;
}
//
//  CGame.cpp
//  indi2dEngine
//
//  Created by Sergey Sergeev on 5/14/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGame.h"
#include "CShaderSerializer_GLSL.h"

CGame::CGame(void)
{
    
}

CGame::~CGame(void)
{
    
}

std::string CGame::_Caller(const std::string &_value)
{
    std::cout<<"[Caller Start Thread] : "<<std::this_thread::get_id()<<std::endl;
    
    CShaderSerializer_GLSL* serializer = new CShaderSerializer_GLSL("ShaderCommon.vert", "ShaderCommon.frag");
    serializer->Serialize();
    
    std::chrono::milliseconds duration(1000);
    std::this_thread::sleep_for(duration);
    
    std::cout<<"[Caller End Thread] : "<<std::this_thread::get_id()<<std::endl;
    
    if(serializer->Get_Status() == E_SERIALIZER_STATUS_SUCCESS)
    {
        std::string value = serializer->Get_VertexShaderSourceCode();
        value.append(serializer->Get_FragmentShaderSourceCode());
        return value;
    }
    return "";
}

void CGame::Call_Test_01(void)
{
    std::cout<<"[Main Thread] : "<<std::this_thread::get_id()<<std::endl;
    
    std::future<std::string> caller1(std::async(&CGame::_Caller, this, "zero"));
    std::string result = caller1.get();
    //std::cout<<result<<std::endl;
    
    std::future<std::string> caller2(std::async(&CGame::_Caller, this, "zero"));
    result = caller2.get();
    //std::cout<<result<<std::endl;
}
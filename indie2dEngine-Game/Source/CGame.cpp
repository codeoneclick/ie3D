//
//  CGame.cpp
//  indi2dEngine
//
//  Created by Sergey Sergeev on 5/14/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGame.h"
#include "CShaderLoader.h"

CGame::CGame(void)
{
    
}

CGame::~CGame(void)
{
    
}

void CGame::Call_Test_01(void)
{
    CShaderLoader* shaderLoader = new CShaderLoader();
    std::shared_ptr<IResource> shader = shaderLoader->StartLoadOperation("ShaderCommon.vert", "ShaderCommon.frag");
    
}

/*
std::string CGame::_Caller(const std::string &_filename)
{
    auto iterator = m_promises.find(_filename);
    if(iterator != m_promises.end())
    {
        return iterator->second->get_future().get();
    }
    std::shared_ptr<std::promise<std::string> > promise = std::make_shared<std::promise<std::string> >();
    m_promises.insert(std::make_pair(_filename, promise));
    return promise->get_future().get();
}

void CGame::_Thread(void)
{
    std::cout<<"[Second Thread] : "<<std::this_thread::get_id()<<std::endl;
    //CShaderSerializer_GLSL* serializer = new CShaderSerializer_GLSL("ShaderCommon.vert", "ShaderCommon.frag");
    //serializer->Serialize();

    for(auto iterator : m_promises)
    {
     //   iterator.second->set_value(serializer->Get_VertexShaderSourceCode());
    }
}

std::shared_ptr<std::future<std::string> > CGame::_Load(const std::string &_filename)
{
    auto iterator = m_operations.find(_filename);
    if(iterator != m_operations.end())
    {
        return iterator->second;
    }
    std::shared_ptr<std::future<std::string> > caller = std::make_shared<std::future<std::string>>(std::async(std::launch::deferred, &CGame::_Caller, this, _filename));
    m_operations.insert(std::make_pair(_filename, caller));
    return caller;
}

void CGame::Call_Test_01(void)
{
    std::cout<<"[Main Thread] : "<<std::this_thread::get_id()<<std::endl;
    m_thread = std::thread(&CGame::_Thread, this);
    
    std::shared_ptr<std::future<std::string> > future = CGame::_Load("one");
    
    std::chrono::milliseconds duration(1000);
    std::this_thread::sleep_for(duration);
    
    std::string result = future->get();
    std::cout<<result<<std::endl;
    std::cout<<"[Main Thread] : "<<std::this_thread::get_id()<<std::endl;
    
    m_promises.insert(std::make_pair("promise1", std::make_shared<std::promise<std::string> >()));
    m_promises.insert(std::make_pair("promise2", std::make_shared<std::promise<std::string> >()));
    
    std::future<std::string> caller1 = m_promises["promise1"]->get_future();
    std::future<std::string> caller2 = m_promises["promise2"]->get_future();
    
   
    
    std::string result = caller1.get();
    std::cout<<result<<std::endl;
    
    std::chrono::milliseconds duration(10000);
    std::this_thread::sleep_for(duration);
    
    std::cout<<"[Main Thread] : "<<std::this_thread::get_id()<<std::endl;
    
    result = caller1.get();
    std::cout<<result<<std::endl;
    
    result = caller2.get();
    std::cout<<result<<std::endl;
    
    //std::future<std::string> caller1(std::async(std::launch::deferred, &CGame::_Caller, this, "zero"));
    //std::string result = caller1.get();
    //std::cout<<result<<std::endl;
    
    //std::future<std::string> caller2(std::async(std::launch::deferred, &CGame::_Caller, this, "zero"));
    //result = caller2.get();
    //std::cout<<result<<std::endl;
}
*/
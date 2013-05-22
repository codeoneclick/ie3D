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
    
    std::map<std::string, std::shared_ptr<std::promise<std::string> > > m_promises;
    std::map<std::string, std::shared_ptr<std::future<std::string> > > m_operations;
    
    std::string _Caller(const std::string& _filename);
    std::thread m_thread;
    
    void _Thread(void);
    std::shared_ptr<std::future<std::string> > _Load(const std::string& _filename);
    
public:
    CGame(void);
    ~CGame(void);
    
    void Call_Test_01(void);
};

#endif 

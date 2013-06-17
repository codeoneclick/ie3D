//
//  thread_concurrency_task.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "thread_concurrency_task.h"

int foooooo()
{
    std::tuple<int, double, char const*> tup(1, 2.0f, "xxx");
    
    std::function<void(int, float, std::string)> function = [](int a, float b, std::string c)
    {
        std::cout<<"call"<<a<<b<<c<<std::endl;
    };
    apply(function, tup);
    //apply(&f, tup);
    //apply(&g, std::make_tuple(4, "yyy"));
};

void f(int p1, double p2, std::string p3)
{
    std::cout << "int=" << p1 << ", double=" << p2 << ", string=" << p3
    << std::endl;
}

void g(int p1, std::string p2)
{
    std::cout << "int=" << p1 << ", string=" << p2 << std::endl;
}
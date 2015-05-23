//
//  CMmap.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/13/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CMmap.h"

i32 CMmap::g_filedescriptors = 0;

CMmap::CMmap(void) : m_filedescriptor(-1),
m_pointer(nullptr)
{
    
};

CMmap::~CMmap(void)
{
    CMmap::deallocate();
};

void* CMmap::allocate(const std::string& filename)
{
    if(m_filedescriptor != -1)
    {
        assert(m_pointer != nullptr);
        return m_pointer;
    }
    ui32 filelength;
    struct stat status;
    
    m_filedescriptor = ::open(filename.c_str(), O_RDWR);
    if (m_filedescriptor < 0)
    {
        std::cout<<"can't open filedescriptor for filename: "<<filename<<"filedescriptors count: "<<g_filedescriptors<<std::endl;
        assert(false);
        return m_pointer;
    }
    g_filedescriptors++;
    
    if (fstat(m_filedescriptor, &status) < 0)
    {
        std::cout<<"can't retrive filedescriptor status for filename: "<<filename<<"filedescriptors count: "<<g_filedescriptors<<std::endl;
        CMmap::deallocate();
        assert(false);
        return m_pointer;
    }
    
    filelength = (ui32)status.st_size;
    m_pointer = (void* )mmap(0, filelength, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, m_filedescriptor, 0);
    if (m_pointer == nullptr)
    {
        std::cout<<"can't mmap filedescriptor for filename: "<<filename<<"filedescriptors count: "<<g_filedescriptors<<std::endl;
        CMmap::deallocate();
        assert(false);
        return m_pointer;
    }
    f32 size = static_cast<f32>(filelength) / (1024.0 * 1024);
    std::cout<<"filedescriptor was allocated: "<<filename<<"; filedescriptors count: "<<g_filedescriptors<<" size: "<<size<<"mb"<<std::endl;
    m_filename = filename;
    return m_pointer;
};

void CMmap::deallocate(void)
{
    if(m_filedescriptor >= 0)
    {
        ::close(m_filedescriptor);
        g_filedescriptors--;
        std::cout<<"filedescriptor was deallocated: "<<m_filename.c_str()<<"; filedescriptors count: "<<g_filedescriptors<<std::endl;
    }
    m_filedescriptor = -1;
    m_pointer = nullptr;
};

void CMmap::reallocate(void)
{
    CMmap::deallocate();
    CMmap::allocate(m_filename);
};
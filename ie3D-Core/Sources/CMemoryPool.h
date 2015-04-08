//
//  CMemoryPool.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 4/6/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CMemoryPool_h
#define CMemoryPool_h

#include "HCommon.h"

namespace ie
{
    class mem_pool
    {
    public:
        
        enum E_POOL_CONSTS
        {
            E_POOL_CONSTS_INIT_SIZE = 0xffffff,
            E_POOL_CONSTS_MIN_SIZE = 0xf
        };
        
    private:
        
        size_t m_size;
        std::list<char *> m_memory;
        
        struct mem_block
        {
            mem_block *m_previous;
            mem_block *m_next;
            size_t m_size;
            bool m_free;
            
            mem_block(mem_block *previous, mem_block *next, size_t size, i32 free):
            m_previous(previous),
            m_next(next),
            m_size(size),
            m_free(free)
            { };
            
            ~mem_block(void) = default;
        };
        
        mem_block* m_blocks;
        
        mem_pool(const mem_pool &) = delete;
        mem_pool& operator= (const mem_pool&) = delete;
        
        void grow(mem_block *block)
        {
            mem_block *allocated = nullptr;
            char *ptr = new char[m_size];
            m_memory.push_back(ptr);
            allocated = reinterpret_cast<mem_block*>(ptr);
            allocated->m_previous = block;
            allocated->m_next = nullptr;
            
            allocated->m_free = 1;
            allocated->m_size= m_size - sizeof(mem_block);
            block->m_next = allocated;
            std::cout<<"[mem pool] -> grow. blocks count: "<<m_memory.size()<<", block size: "<<static_cast<f32>(m_size) / 1024.0<<"kb"<<" [ "<<static_cast<f32>(m_size) / (1024.0 * 1024.0)<<"mb ]"<<std::endl;
        };
        
    public:
        
        mem_pool(size_t size = E_POOL_CONSTS_INIT_SIZE) : m_size(size)
        {
            assert(m_size - sizeof(mem_block) >= E_POOL_CONSTS_MIN_SIZE);
            m_memory.push_back(new char[m_size]);
            m_blocks = reinterpret_cast<mem_block*>(*(m_memory.begin()));
            m_blocks->m_previous = nullptr;
            m_blocks->m_next = nullptr;
            m_blocks->m_free = 1;
            m_blocks->m_size = m_size - sizeof(mem_block);
            std::cout<<"[mem pool] -> grow. blocks count: "<<m_memory.size()<<", block size: "<<static_cast<f32>(m_size) / 1024.0<<"kb"<<"[ "<<static_cast<f32>(m_size) / (1024.0 * 1024.0)<<"mb ]"<<std::endl;
        };
        
        ~mem_pool(void)
        {
            std::for_each(m_memory.begin(), m_memory.end(), [](char *ptr){
                delete [] ptr;
            });
        };
        
        void* allocate(size_t size)
        {
            assert(size <= m_size - sizeof(mem_block));
            mem_block *current_block = m_blocks;
            while(1)
            {
                while(!current_block->m_free)
                {
                    if(!current_block->m_next)
                    {
                        grow(current_block);
                    }
                    current_block = current_block->m_next;
                }
                
                if(current_block->m_size < size)
                {
                    continue;
                }
                break;
            }
            
            if(current_block->m_size - size < 2 * sizeof(mem_block))
            {
                current_block->m_free = false;
                return reinterpret_cast<char *>(current_block) + sizeof(mem_block);
            }
            else
            {
                mem_block* new_block = (reinterpret_cast<mem_block *>(reinterpret_cast<char *>(current_block)
                                                                      + size + sizeof(mem_block)));
                if(current_block->m_next)
                {
                    current_block->m_next->m_previous = new_block;
                }
                new_block->m_next = current_block->m_next;
                current_block->m_next = new_block;
                new_block->m_previous = current_block;
                current_block->m_free = false;
                new_block->m_size = current_block->m_size - size - sizeof(mem_block);
                current_block->m_size = size;
                new_block->m_free = true;
                return reinterpret_cast<char *>(current_block) + sizeof(mem_block);
            }
        };
        
        void deallocate(void *ptr, size_t = 0)
        {
            if(!ptr)
            {
                return;
            }
            mem_block *block = reinterpret_cast<mem_block *>(static_cast<char*>(ptr) - sizeof(mem_block));
            if(block->m_previous && block->m_next)
            {
                if(block->m_previous->m_free && block->m_next->m_free)
                {
                    block->m_previous->m_size += block->m_size + block->m_next->m_size + 2 * sizeof(mem_block);
                    block->m_previous->m_next = block->m_next->m_next;
                    if(block->m_next->m_next)
                    {
                        block->m_next->m_next->m_previous = block->m_previous;
                    }
                    return;
                }
            }
            if(block->m_previous)
            {
                if(block->m_previous->m_free)
                {
                    block->m_previous->m_size += block->m_size + sizeof(mem_block);
                    block->m_previous->m_next = block->m_next;
                    if(block->m_next)
                    {
                        block->m_next->m_previous = block->m_previous;
                    }
                    block->m_free = true;
                    return;
                }
            }
            if(block->m_next)
            {
                if(block->m_next->m_free)
                {
                    block->m_size += block->m_next->m_size + sizeof(mem_block);
                    block->m_next = block->m_next->m_next;
                    if(block->m_next)
                    {
                        block->m_next->m_previous = block;
                    }
                    block->m_free = true;
                    return;
                }
            }
            block->m_free = true;
        };
        
        void dump(void)
        {
            mem_block *block = m_blocks;
            while(1)
            {
                std::cout<<"size="<<block->m_size<<", free="<<block->m_free<<", previous="<<block->m_previous<<", next="<<block->m_next<<std::endl;
                if(block->m_next)
                {
                    block = block->m_next;
                }
                else
                {
                    break;
                }
            }
        };
    };
};

#endif

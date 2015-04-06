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

class CMemoryPool
{
public:
    
    enum E_POOL_INIT
    {
        E_INIT_SIZE = 0xffffff,
        E_MIN_SIZE = 0xf
    };
    
private:
    size_t m_size;
    
    std::list<char *> m_memory;
    
    struct SMemoryBlock
    {
        SMemoryBlock *m_previous;
        SMemoryBlock *m_next;
        size_t m_size;
        i32 m_free;
        
        SMemoryBlock(SMemoryBlock *previous, SMemoryBlock *next, size_t size, i32 free):
        m_previous(previous),
        m_next(next),
        m_size(size),
        m_free(free) { };
        
        ~SMemoryBlock(void) = default;
    };
    
    SMemoryBlock* m_blocks;
    
    CMemoryPool(const CMemoryPool &);
    CMemoryPool& operator=(const CMemoryPool&);
    
    struct killer
    {
        void operator()(char *pointer) { delete [] pointer; };
    };
    
    static void kill(char *pointer) { delete []pointer; };
    
    void grow(SMemoryBlock *block)
    {
        SMemoryBlock *allocated = nullptr;
        char *pointer = new char[m_size];
        m_memory.push_back(pointer);
        allocated = reinterpret_cast<SMemoryBlock*>(pointer);
        allocated->m_previous = block;
        allocated->m_next = nullptr;
        allocated->m_free = 1;
        allocated->m_size= m_size - sizeof(SMemoryBlock);
        block->m_next = allocated;
    };
    
public:
    
    CMemoryPool(size_t size = E_INIT_SIZE) : m_size(size)
    {
        assert(m_size - sizeof(SMemoryBlock) >= E_MIN_SIZE);
        m_memory.push_back(new char[m_size]);
        m_blocks = reinterpret_cast<SMemoryBlock*>(*(m_memory.begin()));
        m_blocks->m_previous = nullptr;
        m_blocks->m_next = nullptr;
        m_blocks->m_free = 1;
        m_blocks->m_size = m_size - sizeof(SMemoryBlock);
    };
    
    ~CMemoryPool(void)
    {
        std::for_each(m_memory.begin(), m_memory.end(), killer());
    };
    
    void* allocate(size_t size)
    {
        assert(size <= m_size- sizeof(SMemoryBlock));
        SMemoryBlock *block = m_blocks;
        while(1)
        {
            while(!block->m_free)
            {
                if(!block->m_next)
                {
                    grow(block);
                }
                block = block->m_next;
            }
            
            if(block->m_size < size)
            {
                continue;
            }
            break;
        }
        
        if(block->m_size - size < 2 * sizeof(SMemoryBlock))
        {
            block->m_free = 0;
            return reinterpret_cast<char *>(block) + sizeof(SMemoryBlock);
        }
        else
        {
            SMemoryBlock* allocated = (reinterpret_cast<SMemoryBlock *>(reinterpret_cast<char *>(block)
                                                                        + size + sizeof(SMemoryBlock)));
            if(block->m_next)
            {
                block->m_next->m_previous = allocated;
            }
            allocated->m_next = block->m_next;
            block->m_next = allocated;
            allocated->m_previous = block;
            block->m_free = 0;
            allocated->m_size = block->m_size - size - sizeof(SMemoryBlock);
            block->m_size = size;
            allocated->m_free = 1;
            return reinterpret_cast<char *>(block) + sizeof(SMemoryBlock);
        }
    };
    
    void deallocate(void *pointer, size_t = 0)
    {
        if(!pointer)
        {
            return;
        }
        SMemoryBlock *block = reinterpret_cast<SMemoryBlock *>(static_cast<char*>(pointer) - sizeof(SMemoryBlock));
        if(block->m_previous && block->m_next)
        {
            if(block->m_previous->m_free && block->m_next->m_free)
            {
                block->m_previous->m_size += block->m_size + block->m_next->m_size + 2 * sizeof(SMemoryBlock);
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
                block->m_previous->m_size += block->m_size + sizeof(SMemoryBlock);
                block->m_previous->m_next = block->m_next;
                if(block->m_next)
                {
                    block->m_next->m_previous = block->m_previous;
                }
                block->m_free= 1;
                return;
            }
        }
        if(block->m_next)
        {
            if(block->m_next->m_free)
            {
                block->m_size += block->m_next->m_size + sizeof(SMemoryBlock);
                block->m_next = block->m_next->m_next;
                if(block->m_next)
                {
                    block->m_next->m_previous = block;
                }
                block->m_free = 1;
                return;
            }
        }
        block->m_free = 1;
    };
    
    void dump(void)
    {
        SMemoryBlock *block = m_blocks;
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

#endif

//
//  CMemoryAllocator.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 4/6/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CMemoryAllocator_h
#define CMemoryAllocator_h

#include "CMemoryPool.h"

namespace ie
{
    template <typename T> class mem_allocator;
    
    template <> class mem_allocator<void>
    {
    public:
        
        typedef void* pointer;
        typedef const void* const_pointer;
        
        typedef void value_type;
        template <class U>
        struct rebind { typedef mem_allocator<U> other; };
    };
    
    namespace pool
    {
        inline void destruct(char *) { };
        inline void destruct(wchar_t*) { };
        template <typename T>
        inline void destruct(T *value) { value->~T(); };
    }
    
    template <typename T>
    class mem_allocator
    {
    private:
        
        static mem_pool m_pool;
        
    public:
        
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T value_type;
        
        template <class U>
        struct rebind { typedef mem_allocator<U> other; };
        
        mem_allocator(void) = default;
        
        pointer address(reference ref) const {return &ref; };
        const_pointer address(const_reference ref) const {return &ref;}
        pointer allocate(size_type size, mem_allocator<void>::const_pointer hint = 0)
        {
            return static_cast<pointer>(m_pool.allocate(size * sizeof(T)));
        };
        
        template <class U> mem_allocator(const mem_allocator<U>&) { };
        void deallocate(pointer p, size_type n)
        {
            m_pool.deallocate(p, n);
        };
        
        void deallocate(void *p, size_type n)
        {
            m_pool.deallocate(p, n);
        };
        
        size_type max_size() const throw() { return size_t(-1) / sizeof(value_type); }
        
        void construct(pointer p, const T& value)
        {
            new(static_cast<void*>(p)) T(value);
        };
        
        void construct(pointer p)
        {
            new(static_cast<void*>(p)) T();
        };
        
        void destroy(pointer p) { pool::destruct(p); };
        
        static void dump(void) { m_pool.dump(); };
        
    };
    
    template <typename T> mem_pool mem_allocator<T>::m_pool;
    
    template <typename T, typename U>
    inline bool operator == (const mem_allocator<T>&, const mem_allocator<U>) { return true; };
    
    template <typename T, typename U>
    inline bool operator != (const mem_allocator<T>&, const mem_allocator<U>) { return false; };
    
};

#endif

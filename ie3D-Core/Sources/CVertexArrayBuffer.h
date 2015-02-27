//
//  CVertexArrayBuffer.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 2/26/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CVertexArrayBuffer_h
#define CVertexArrayBuffer_h

#include "HCommon.h"
#include "HDeclaration.h"
#include "HEnums.h"

class CVertexArrayBuffer
{
private:
    
protected:
    
    ui32 m_vao;
    
    CSharedVertexBuffer m_vertexBuffer;
    CSharedIndexBuffer m_indexBuffer;
    
public:
    
    CVertexArrayBuffer(CSharedVertexBufferRef vertexBuffer,
                       CSharedIndexBufferRef indexBuffer);
    ~CVertexArrayBuffer(void);
    
    void init(const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes);
    
    static void bind(CSharedVertexArrayBufferRef vaoState);
    static void unbind(void);
};

#endif

//
//  CTextureCommiter_PVR.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CTextureCommiter_PVR_h
#define CTextureCommiter_PVR_h

#include "IResourceCommiter.h"

class CTextureCommiter_PVR final : public IResourceCommiter
{
private:
    
protected:
    
    std::string m_vsSourceCode;
    std::string m_fsSourceCode;
    
    ui32 _Compile(const std::string& _sourceCode, GLenum _shader);
    ui32 _Link(ui32 _vsHandle, ui32 _fsHandle);
    
public:
    
    CTextureCommiter_PVR(const std::string& _guid, const std::string& _vsSourceCode, const std::string& _fsSourceCode, std::shared_ptr<IResource> _resource);
    ~CTextureCommiter_PVR(void);
    
    void Commit(void);
};


#endif 

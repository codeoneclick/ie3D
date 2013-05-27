//
//  CMeshCommiter_MDL.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CMeshCommiter_MDL_h
#define CMeshCommiter_MDL_h

#include "IResourceCommiter.h"

class CMeshHeader;
class CMeshCommiter_MDL final : public IResourceCommiter
{
private:
    
protected:
    
    std::shared_ptr<CMeshHeader> m_header;
    
public:
    
    CMeshCommiter_MDL(const std::string& _guid, std::shared_ptr<CMeshHeader> _header,  std::shared_ptr<IResource> _resource);
    ~CMeshCommiter_MDL(void);
    
    void Commit(void);
};


#endif

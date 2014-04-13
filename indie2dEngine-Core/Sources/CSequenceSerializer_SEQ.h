//
//  CSequenceSerializer_SEQ.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CSequenceSerializer_SEQ_h
#define CSequenceSerializer_SEQ_h

#include "IResourceSerializer.h"

class CSequenceSerializer_SEQ final : public IResourceSerializer
{
private:
    
protected:
    
    std::string m_filename;
    
public:
    
    CSequenceSerializer_SEQ(const std::string& filename,
                            ISharedResourceRef resource);
    ~CSequenceSerializer_SEQ(void);
    
    void serialize(void);
};


#endif 

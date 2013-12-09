//
//  CSequenceSerializer_SEQ.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSequenceSerializer_SEQ.h"
#include "CSequence.h"

CSequenceSerializer_SEQ::CSequenceSerializer_SEQ(const std::string& _filename, std::shared_ptr<IResource> _resource) :
IResourceSerializer(_filename, _resource),
m_filename(_filename)
{
    
}

CSequenceSerializer_SEQ::~CSequenceSerializer_SEQ(void)
{
    
}

void CSequenceSerializer_SEQ::Serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_SERIALIZER_STATUS_INPROGRESS;
    
    std::istream* filestream = IResourceSerializer::_LoadData(m_filename);
    std::shared_ptr<CSequence> sequence = std::static_pointer_cast<CSequence >(m_resource);
    sequence->_Serialize(filestream);
    IResourceSerializer::_FreeData(filestream);
    
    m_status = E_SERIALIZER_STATUS_SUCCESS;
}
//
//  CMeshSerializer_MDL.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMeshSerializer_MDL.h"
#include "CCommonOS.h"
#include "CMesh.h"
#include "PVRTTexture.h"

CMeshSerializer_MDL::CMeshSerializer_MDL(const std::string& _filename, std::shared_ptr<IResource> _resource) :
IResourceSerializer(_filename, _resource),
m_filename(_filename)
{
    
}

CMeshSerializer_MDL::~CMeshSerializer_MDL(void)
{
    
}

void CMeshSerializer_MDL::Serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_SERIALIZER_STATUS_INPROGRESS;
    
    std::string path(Get_BundlePath());
    std::string filename(path);
    filename.append(m_filename);
    
    std::ifstream filestream;
    filestream.open(filename.c_str());
    if (!filestream.is_open())
    {
        m_status = E_SERIALIZER_STATUS_FAILURE;
        return;
    }
}
//
//  CMeshLoader_Testing.c
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/27/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "gtest/gtest.h"
#include "CMesh.h"
#include "CMeshSerializer_MDL.h"
#include "CMeshCommiter_MDL.h"
#include "CMeshLoader.h"

const std::string g_filename = "mesh.mdl";

TEST(CMeshLoader, StartLoadingOperation)
{
    CMeshLoader* meshLoader = new CMeshLoader();
    std::shared_ptr<IResource> mesh = meshLoader->StartLoadOperation(g_filename);
    EXPECT_NE(mesh, nullptr);
}

TEST(CMeshSerializer_MDL, Serialize)
{
    std::shared_ptr<IResource> resource = std::make_shared<CMesh>(g_filename);
    std::shared_ptr<CMeshSerializer_MDL> serializer = std::make_shared<CMeshSerializer_MDL>(g_filename,
                                                                                            resource);
    serializer->Serialize();
    EXPECT_TRUE(serializer->Get_Status() == E_SERIALIZER_STATUS_SUCCESS);
    EXPECT_TRUE(resource->IsLoaded());
}

TEST(CMeshCommiter_MDL, Commit)
{
    std::shared_ptr<CMesh> mesh = std::make_shared<CMesh>(g_filename);
    std::shared_ptr<CMeshSerializer_MDL> serializer = std::make_shared<CMeshSerializer_MDL>(g_filename,
                                                                                               mesh);
    serializer->Serialize();
    EXPECT_TRUE(serializer->Get_Status() == E_SERIALIZER_STATUS_SUCCESS);
    EXPECT_TRUE(mesh->IsLoaded());
    
    std::shared_ptr<CMeshCommiter_MDL> commiter = std::make_shared<CMeshCommiter_MDL>(g_filename,
                                                                                      mesh->_Get_Header(),
                                                                                      mesh);
    commiter->Commit();
    EXPECT_TRUE(commiter->Get_Status() == E_COMMITER_STATUS_SUCCESS);
    EXPECT_TRUE(mesh->IsLinked());
}
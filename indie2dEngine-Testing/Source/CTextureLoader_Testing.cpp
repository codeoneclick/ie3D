//
//  CTextureLoader_Testing.c
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "gtest/gtest.h"
#include "CTexture.h"
#include "CTextureSerializer_PVR.h"
#include "CTextureCommiter_PVR.h"
#include "CTextureLoader.h"

const std::string g_filename = "texture_01.pvr";

TEST(CTextureLoader, StartLoadingOperation)
{
    CTextureLoader* textureLoader = new CTextureLoader();
    std::shared_ptr<IResource> texture = textureLoader->StartLoadOperation(g_filename);
    EXPECT_NE(texture, nullptr);
}

TEST(CTextureSerializer_PVR, Serialize)
{
    std::shared_ptr<IResource> resource = std::make_shared<CTexture>(g_filename);
    std::shared_ptr<CTextureSerializer_PVR> serializer = std::make_shared<CTextureSerializer_PVR>(g_filename,
                                                                                                  resource);
    serializer->Serialize();
    EXPECT_TRUE(serializer->Get_Status() == E_SERIALIZER_STATUS_SUCCESS);
    EXPECT_TRUE(resource->IsLoaded());
}

TEST(CTextureCommiter_PVR, Commit)
{
    std::shared_ptr<CTexture> texture = std::make_shared<CTexture>(g_filename);
    std::shared_ptr<CTextureSerializer_PVR> serializer = std::make_shared<CTextureSerializer_PVR>(g_filename,
                                                                                                  texture);
    serializer->Serialize();
    EXPECT_TRUE(serializer->Get_Status() == E_SERIALIZER_STATUS_SUCCESS);
    EXPECT_TRUE(texture->IsLoaded());
    
    std::shared_ptr<CTextureCommiter_PVR> commiter = std::make_shared<CTextureCommiter_PVR>(g_filename,
                                                                                   texture->_Get_Header(),
                                                                                   texture);
    commiter->Commit();
    EXPECT_TRUE(commiter->Get_Status() == E_COMMITER_STATUS_SUCCESS);
    EXPECT_TRUE(texture->IsLinked());
}
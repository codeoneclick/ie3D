//
//  CShaderLoader_Testing.c
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "gtest/gtest.h"
#include "CShaderLoader.h"
#include "CShaderLoadingOperation.h"
#include "CShaderSerializer_GLSL.h"
#include "CShaderCommiter_GLSL.h"
#include "CShader.h"

const std::string g_vsFilename = "ShaderCommon.vert";
const std::string g_fsFilename = "ShaderCommon.frag";

TEST(CShaderLoader, StartLoadingOperation)
{
    CShaderLoader* shaderLoader = new CShaderLoader();
    std::shared_ptr<IResource> shader = shaderLoader->StartLoadOperation(g_vsFilename, g_fsFilename);
    EXPECT_NE(shader, nullptr);
}

TEST(CShaderSerializer_GLSL, Serialize)
{
    std::shared_ptr<CShader> texture = std::make_shared<CShader>(std::string().append(g_vsFilename).append(g_fsFilename));
    std::shared_ptr<CShaderSerializer_GLSL> serializer = std::make_shared<CShaderSerializer_GLSL>(g_vsFilename,
                                                                                                  g_fsFilename,
                                                                                                  texture);
    serializer->Serialize();
    EXPECT_TRUE(serializer->Get_Status() == E_SERIALIZER_STATUS_SUCCESS);
    EXPECT_TRUE(texture->IsLoaded());
}

TEST(CShaderCommiter_GLSL, Commit)
{
    std::shared_ptr<CShader> shader = std::make_shared<CShader>(std::string().append(g_vsFilename).append(g_fsFilename));
    std::shared_ptr<CShaderSerializer_GLSL> serializer = std::make_shared<CShaderSerializer_GLSL>(g_vsFilename,
                                                                                                  g_fsFilename,
                                                                                                  shader);
    serializer->Serialize();
    EXPECT_TRUE(serializer->Get_Status() == E_SERIALIZER_STATUS_SUCCESS);
    EXPECT_TRUE(shader->IsLoaded());
    
    std::shared_ptr<CShaderCommiter_GLSL> commiter = std::make_shared<CShaderCommiter_GLSL>(serializer->Get_Guid(),
                                                        shader->_Get_VertexShaderSourceCode(),
                                                        shader->_Get_FragmentShaderSourceCode(),
                                                        shader);

    commiter->Commit();
    EXPECT_TRUE(commiter->Get_Status() == E_COMMITER_STATUS_SUCCESS);
    EXPECT_TRUE(shader->IsLinked());
}

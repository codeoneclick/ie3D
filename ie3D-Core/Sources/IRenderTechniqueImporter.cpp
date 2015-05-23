//
//  IRenderTechniqueImporter.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/2/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "IRenderTechniqueImporter.h"
#include "CRenderTechniqueMain.h"
#include "CRenderTechniqueWorldSpace.h"
#include "CRenderTechniqueScreenSpace.h"
#include "IGraphicsContext.h"
#include "CRenderTarget.h"
#include "CShader.h"
#include "CMaterial.h"
#include "CQuad.h"
#include "CTexture.h"
#include "COcclusionQueryManager.h"

#if defined(__OSX__)

#include <Cocoa/Cocoa.h>
#include <QuartzCore/QuartzCore.h>

#elif defined(__IOS__)

#include <UIKit/UIKit.h>
#include <QuartzCore/QuartzCore.h>

#endif

IRenderTechniqueImporter::IRenderTechniqueImporter(ISharedGraphicsContextRef graphicsContext, bool isOffscreen) :
m_graphicsContext(graphicsContext),
m_isOffscreen(isOffscreen),
m_mainRenderTechnique(nullptr)
{
    assert(m_graphicsContext != nullptr);
    m_occlusionQueryManager = std::make_shared<COcclusionQueryManager>();
}

IRenderTechniqueImporter::~IRenderTechniqueImporter(void)
{
    
}

void IRenderTechniqueImporter::setMainRenderTechnique(CSharedMaterialRef material)
{
    assert(m_isOffscreen == false);
    assert(m_graphicsContext != nullptr);
    assert(material != nullptr);
    m_mainRenderTechnique = std::make_shared<CRenderTechniqueMain>(m_graphicsContext->getWidth(),
                                                                   m_graphicsContext->getHeight(),
                                                                   material,
                                                                   m_graphicsContext->getFrameBuffer(),
                                                                   m_graphicsContext->getRenderBuffer());
#if defined(__IOS__)
    const auto& platform = g_platforms.find(getPlatform());
    if(platform == g_platforms.end())
    {
        std::cout<<"[Device] : Simulator"<<std::endl;
    }
    else
    {
        std::cout<<"[Device] : "<<platform->second<<std::endl;
    }
#endif
    std::cout<<"[Output resolution] : "<<m_graphicsContext->getWidth()<<"x"<<m_graphicsContext->getHeight()<<std::endl;
    std::cout<<"["<<glGetString(GL_RENDERER)<<"] ["<<glGetString(GL_VERSION)<<"] ["<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<"]"<<std::endl;
}

void IRenderTechniqueImporter::addWorldSpaceRenderTechnique(const std::string& techniqueName, CSharedRenderTechniqueWorldSpaceRef technique)
{
    assert(m_worldSpaceRenderTechniques.find(techniqueName) == m_worldSpaceRenderTechniques.end());
    m_worldSpaceRenderTechniques.insert(std::make_pair(techniqueName, technique));
}

void IRenderTechniqueImporter::removeWorldSpaceRenderTechnique(const std::string& techniqueName)
{
    const auto& iterator = m_worldSpaceRenderTechniques.find(techniqueName);
    assert(iterator != m_worldSpaceRenderTechniques.end());
    m_worldSpaceRenderTechniques.erase(iterator);
}

void IRenderTechniqueImporter::addScreenSpaceRenderTechnique(const std::string& techniqueName, CSharedRenderTechniqueScreenSpaceRef technique)
{
    assert(m_screenSpaceRenderTechniques.find(techniqueName) == m_screenSpaceRenderTechniques.end());
    m_screenSpaceRenderTechniques.insert(std::make_pair(techniqueName, technique));
}

void IRenderTechniqueImporter::removeScreenSpaceRenderTechnique(const std::string& techniqueName)
{
    const auto& iterator = m_screenSpaceRenderTechniques.find(techniqueName);
    assert(iterator != m_screenSpaceRenderTechniques.end());
    m_screenSpaceRenderTechniques.erase(iterator);
}

void IRenderTechniqueImporter::addRenderTechniqueHandler(const std::string& techniqueName, ISharedRenderTechniqueHandlerRef handler)
{
    const auto& iterator = m_worldSpaceRenderTechniques.find(techniqueName);
    assert(iterator != m_worldSpaceRenderTechniques.end());
    iterator->second->addRenderTechniqueHandler(handler);
}

void IRenderTechniqueImporter::removeRenderTechniqueHandler(const std::string& techniqueName, ISharedRenderTechniqueHandlerRef handler)
{
    const auto& iterator = m_worldSpaceRenderTechniques.find(techniqueName);
    assert(iterator != m_worldSpaceRenderTechniques.end());
    iterator->second->removeRenderTechniqueHandler(handler);
}

bool IRenderTechniqueImporter::isSupportingRenderTechnique(const std::string& techniqueName)
{
    const auto& iterator = m_worldSpaceRenderTechniques.find(techniqueName);
    return iterator != m_worldSpaceRenderTechniques.end();
}

void IRenderTechniqueImporter::addRenderTechninqueOperationTextureHandler(const std::string& techniqueName,
                                                                          ISharedRenderTechniqueOperationTextureHandlerRef handler)
{
    auto iterator = m_renderTechniqueOperationTextureHandlers.find(techniqueName);
    if(iterator == m_renderTechniqueOperationTextureHandlers.end())
    {
        std::set<ISharedRenderTechniqueOperationTextureHandler> handlers;
        m_renderTechniqueOperationTextureHandlers.insert(std::make_pair(techniqueName, handlers));
        iterator = m_renderTechniqueOperationTextureHandlers.find(techniqueName);
    }
    assert(iterator != m_renderTechniqueOperationTextureHandlers.end());
    iterator->second.insert(handler);
}

void IRenderTechniqueImporter::removeRenderTechninqueOperationTextureHandler(ISharedRenderTechniqueOperationTextureHandlerRef handler)
{
    for(auto iterator : m_renderTechniqueOperationTextureHandlers)
    {
        iterator.second.erase(handler);
    }
}

void IRenderTechniqueImporter::saveTexture(CSharedTextureRef texture, const std::string& filename, ui32 width, ui32 height)
{
    assert(texture != nullptr);
    assert(texture->isLoaded() && texture->isCommited());
    
    CSharedMaterial material = std::make_shared<CMaterial>();
    CSharedShader shader = CShader::constructCustomShader("texture2D", ShaderTexure2D_vert, ShaderTexure2D_frag);
    assert(shader != nullptr);
    CSharedQuad quad = std::make_shared<CQuad>();
    material->setShader(shader);
    material->setTexture(texture, E_SHADER_SAMPLER_01);
    
    material->setCulling(false);
    material->setCullingMode(GL_BACK);
    
    material->setBlending(false);
    material->setBlendingFunctionSource(GL_SRC_ALPHA);
    material->setBlendingFunctionDestination(GL_ONE);
    
    material->setDepthTest(false);
    material->setDepthMask(true);
    
    material->setClipping(false);
    material->setClippingPlane(glm::vec4(0.0, 0.0, 0.0, 0.0));
    
    material->setReflecting(false);
    material->setShadowing(false);
    material->setDebugging(false);
    
    CSharedRenderTarget renderTarget = std::make_shared<CRenderTarget>(m_graphicsContext, GL_RGBA, width, height);
    
    renderTarget->begin();
    renderTarget->clear();
    
    material->bind();
    assert(material->getShader()->getAttributes().at(E_SHADER_ATTRIBUTE_POSITION) >= 0);
    assert(material->getShader()->getAttributes().at(E_SHADER_ATTRIBUTE_TEXCOORD) >= 0);
    quad->bind(material->getShader()->getGUID(), material->getShader()->getAttributes());
    
    quad->draw();
    
    quad->unbind(material->getShader()->getGUID(), material->getShader()->getAttributes());
    material->unbind();
    
    ui32 rawdataSize = static_cast<ui32>(width) * static_cast<ui32>(height) * 4;
    ui8 *rawdata = new ui8[rawdataSize];
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, rawdata);
    
    renderTarget->end();
    
#if defined(__OSX__)
    
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, rawdata, rawdataSize, NULL);
    ui32 bitsPerComponent = 8;
    ui32 bitsPerPixel = 32;
    ui32 bytesPerRow = 4 * width;
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGImageAlphaPremultipliedLast;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
    CGImageRef image = CGImageCreate(width,
                                     height,
                                     bitsPerComponent,
                                     bitsPerPixel,
                                     bytesPerRow,
                                     colorSpaceRef,
                                     bitmapInfo,
                                     provider, NULL, NO, renderingIntent);
    
    CFURLRef url = (__bridge CFURLRef)[NSURL fileURLWithPath:[NSString stringWithUTF8String:filename.c_str()]];
    CGImageDestinationRef destination = CGImageDestinationCreateWithURL(url, kUTTypePNG, 1, NULL);
    CGImageDestinationAddImage(destination, image, nil);
    
    if (!CGImageDestinationFinalize(destination))
    {
        assert(false);
    }
    CFRelease(destination);
    
#elif defined(__IOS__)
    
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, rawdata, rawdataSize, NULL);
    
    ui32 bitsPerComponent = 8;
    ui32 bitsPerPixel = 32;
    ui32 bytesPerRow = 4 * width;
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGImageAlphaPremultipliedLast;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
    CGImageRef imageRef = CGImageCreate(width,
                                        height,
                                        bitsPerComponent,
                                        bitsPerPixel,
                                        bytesPerRow,
                                        colorSpaceRef,
                                        bitmapInfo,
                                        provider, NULL, NO, renderingIntent);
    UIImage *image = [UIImage imageWithCGImage:imageRef];
    
    UIImageView *imageView = [[UIImageView alloc] initWithImage:image];
    UIGraphicsBeginImageContext(CGSizeMake(width, height));
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGAffineTransform flip = CGAffineTransformMake(1, 0, 0, -1, 0, width);
    CGContextConcatCTM(context, flip);
    [imageView.layer renderInContext:context];
    image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *imageFilePath = [[paths objectAtIndex:0] stringByAppendingPathComponent:
                               [NSString stringWithCString:filename.c_str()
                                                  encoding:[NSString defaultCStringEncoding]]];
    [UIImagePNGRepresentation(image) writeToFile:imageFilePath atomically:YES];
    
#endif
    
    delete[] rawdata;
}

void IRenderTechniqueImporter::addToOcluddingQuery(const std::string& techniqueName, ISharedOcclusionQueryHandlerRef handler)
{
    assert(m_occlusionQueryManager != nullptr);
    m_occlusionQueryManager->addToOcluddingQuery(techniqueName, handler);
}

void IRenderTechniqueImporter::removeFromOcluddingQuery(const std::string& techniqueName, ISharedOcclusionQueryHandlerRef handler)
{
    assert(m_occlusionQueryManager != nullptr);
    m_occlusionQueryManager->removeFromOcluddingQuery(techniqueName, handler);
}

//
//  CEditableSceneTransition.cpp
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#include "CEditableSceneTransition.h"
#include "CEditableScene.h"

CEditableSceneTransition::CEditableSceneTransition(const std::string& filename,
                                                   std::shared_ptr<IGraphicsContext> graphicsContext,
                                                   std::shared_ptr<IInputContext> gestureRecognizerContext,
                                                   std::shared_ptr<CResourceAccessor> resourceAccessor,
                                                   std::shared_ptr<CConfigurationAccessor> configurationAccessor) :
IFabricator(configurationAccessor, resourceAccessor),
IGameTransition(filename,
                graphicsContext,
                gestureRecognizerContext,
                resourceAccessor,
                configurationAccessor)
{
    
}

CEditableSceneTransition::~CEditableSceneTransition(void)
{
    
}

void CEditableSceneTransition::_OnLoaded(void)
{
    m_scene = std::make_shared<CEditableScene>(this);
    m_scene->load();
    m_isLoaded = true;
}

void CEditableSceneTransition::_OnGameLoopUpdate(f32 deltatime)
{
    if(m_isLoaded && m_scene != nullptr)
    {
        m_scene->update(deltatime);
    }
}
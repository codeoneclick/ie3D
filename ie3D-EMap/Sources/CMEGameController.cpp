//
//  CMEGameController.cpp
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#include "CMEGameController.h"
#include "IGraphicsContext.h"
#include "IInputContext.h"
#include "CMEMainSceneTransition.h"
#include "CMEModelsSceneTransition.h"
#include "CConfigurationAccessor.h"
#include "CMEConfigurationAccessor.h"

CMEGameController::CMEGameController(ISharedOGLWindowRef window) : IGameController(window)
{
     m_configurationAccessor = std::make_shared<CMEConfigurationAccessor>();
}
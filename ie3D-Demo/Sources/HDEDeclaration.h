//
//  HDEDeclaration.h
//  ie3D-Demo
//
//  Created by sergey.sergeev on 8/12/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef HDEDeclaration_h
#define HDEDeclaration_h

#include "HEDeclaration.h"

class IGameObjectNavigatorHandler;
typedef std::shared_ptr<IGameObjectNavigatorHandler> ISharedGameObjectNavigatorHandler;
typedef const ISharedGameObjectNavigatorHandler& ISharedGameObjectNavigatorHandlerRef;

class CGameObjectNavigator;
typedef std::shared_ptr<CGameObjectNavigator> CSharedGameObjectNavigator;
typedef const CSharedGameObjectNavigator& CSharedGameObjectNavigatorRef;

class CDEUIToSceneCommands;
typedef std::shared_ptr<CDEUIToSceneCommands> CDESharedUIToSceneCommands;
typedef const CDESharedUIToSceneCommands& CDESharedUIToSceneCommandsRef;

class CDESceneToUICommands;
typedef std::shared_ptr<CDESceneToUICommands> CDESharedSceneToUICommands;
typedef const CDESharedSceneToUICommands& CDESharedSceneToUICommandsRef;

class ICharacterController;
typedef std::shared_ptr<ICharacterController> ISharedCharacterController;
typedef const ISharedCharacterController& ISharedCharacterControllerRef;

#endif

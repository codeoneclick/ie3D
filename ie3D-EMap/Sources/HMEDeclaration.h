//
//  HMEDecaration.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 6/9/14.
//
//

#ifndef HMEDecaration_h
#define HMEDecaration_h

#include "HEDeclaration.h"

class CMELandscapeBrush;
typedef std::shared_ptr<CMELandscapeBrush> CMESharedLandscapeBrush;
typedef const CMESharedLandscapeBrush& CMESharedLandscapeBrushRef;

class CMEModelBrush;
typedef std::shared_ptr<CMEModelBrush> CMESharedModelBrush;
typedef const CMESharedModelBrush& CMESharedModelBrushRef;

class CMEUIToSceneCommands;
typedef std::shared_ptr<CMEUIToSceneCommands> CSharedMEUIToSceneCommands;
typedef const CSharedMEUIToSceneCommands& CSharedMEUIToSceneCommandsRef;

class CMESceneToUICommands;
typedef std::shared_ptr<CMESceneToUICommands> CSharedMESceneToUICommands;
typedef const CSharedMESceneToUICommands& CSharedMESceneToUICommandsRef;

#endif

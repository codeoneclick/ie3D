//
//  HMEDecaration.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 6/9/14.
//
//

#ifndef HMEDecaration_h
#define HMEDecaration_h

#include "HDeclaration.h"

class CMELandscapeBrush;
typedef std::shared_ptr<CMELandscapeBrush> CSharedMELandscapeBrush;
typedef const CSharedMELandscapeBrush& CSharedMELandscapeBrushRef;

class CMETankComplex;
typedef std::shared_ptr<CMETankComplex> CSharedMETankComplex;
typedef const CSharedMETankComplex& CSharedMETankComplexRef;

class CMEUIToSceneCommands;
typedef std::shared_ptr<CMEUIToSceneCommands> CSharedMEUIToSceneCommands;
typedef const CSharedMEUIToSceneCommands& CSharedMEUIToSceneCommandsRef;

class CMESceneToUICommands;
typedef std::shared_ptr<CMESceneToUICommands> CSharedMESceneToUICommands;
typedef const CSharedMESceneToUICommands& CSharedMESceneToUICommandsRef;

#endif

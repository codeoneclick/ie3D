//
//  HEDeclaration.h
//  ie3D-Engine
//
//  Created by sergey.sergeev on 8/6/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#ifndef HEDeclaration_h
#define HEDeclaration_h

#include "HDeclaration.h"

class CEComplexModel;
typedef std::shared_ptr<CEComplexModel> CESharedComplexModel;
typedef const CESharedComplexModel& CESharedComplexModelRef;

class CECustomModel;
typedef std::shared_ptr<CECustomModel> CESharedCustomModel;
typedef const CESharedCustomModel& CESharedCustomModelRef;

#endif

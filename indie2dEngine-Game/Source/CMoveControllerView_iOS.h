//
//  CMoveControllerView_iOS.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CMoveControllerView_iOS_h
#define CMoveControllerView_iOS_h

#include <UIKit/UIKit.h>
#include "HCommon.h"

class IMoveControllerHandler;

@interface CMoveControllerView_iOS : UIView

@end


extern void RegisterMoveControllerHandler(std::shared_ptr<IMoveControllerHandler> _handler);
extern void UnregisterMoveControllerHandler(std::shared_ptr<IMoveControllerHandler> _handler);

#endif
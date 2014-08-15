//
//  CDEJoystick.h
//  ie3D-Demo
//
//  Created by sergey.sergeev on 8/15/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#import <UIKit/UIKit.h>

@class CDEJoystick;
@protocol CDEJoystickDelegate <NSObject>

@optional

- (void)joystick:(CDEJoystick *)aJoystick startMoveAtPoint:(CGPoint)point;
- (void)joystick:(CDEJoystick *)aJoystick endMoveAtPoint:(CGPoint)point;

@end

@interface CDEJoystick : UIView

@property (nonatomic, assign) CGRect activeBounds;
@property (nonatomic, weak) id<CDEJoystickDelegate> delegate;

@end

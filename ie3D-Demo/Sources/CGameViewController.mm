//
//  CGameViewController.m
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameViewController.h"
#include "IOGLWindow.h"
#include "CDemoGameController.h"
#include "CDemoGameTransition.h"
#include "CGameLoopExecutor.h"
#include "CDEUIToSceneCommands.h"
#include "CDEJoystick.h"

@interface CGameViewController () <CDEJoystickDelegate>

@property (weak, nonatomic) IBOutlet OpenGLView *m_openglView;
@property (unsafe_unretained, nonatomic) std::string result;
@property (weak, nonatomic) IBOutlet UILabel *fpsLabel;
@property (weak, nonatomic) IBOutlet UILabel *trianglesLabel;
@property (weak, nonatomic) IBOutlet CDEJoystick *joystickView;

@property (nonatomic, unsafe_unretained) std::shared_ptr<CDemoGameController> gameController;
@property (nonatomic, unsafe_unretained) std::shared_ptr<IGameTransition> gameTransition;

@end

@implementation CGameViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    f32 width = MAX_VALUE([[UIScreen mainScreen] bounds].size.height, [[UIScreen mainScreen] bounds].size.width);
    f32 height = MIN_VALUE([[UIScreen mainScreen] bounds].size.height, [[UIScreen mainScreen] bounds].size.width);
    [self.view setFrame:CGRectMake(0.0f, 0.0f, width, height)];
    
    NSMethodSignature* signature = [self methodSignatureForSelector:@selector(onTick:)];
    NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
    [invocation setTarget: self];
    [invocation setSelector:@selector(onTick:)];
    
    NSTimer *sender = [NSTimer timerWithTimeInterval:0.1 invocation:invocation repeats:YES];
    NSRunLoop *runner = [NSRunLoop currentRunLoop];
    [runner addTimer:sender forMode:NSDefaultRunLoopMode];
    
    std::shared_ptr<IOGLWindow> window = std::make_shared<IOGLWindow>((__bridge void*)self.m_openglView);
    
    self.gameController = std::make_shared<CDemoGameController>();
    self.gameTransition = self.gameController->CreateKOTHInGameTransition("transition.main.xml", window);
    self.gameController->RegisterTransition(self.gameTransition);
    self.gameController->GoToTransition("transition.main.xml");

    self.joystickView.delegate = self;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown && interfaceOrientation != UIInterfaceOrientationPortrait);
}

- (void)viewDidUnload
{
    [self setFpsLabel:nil];
    [self setTrianglesLabel:nil];
    [super viewDidUnload];
}

- (void)onTick:(NSTimer*)sender
{
    [self.fpsLabel setText:[NSString stringWithFormat:@"FPS: %i", Get_FramesPerSecond()]];
    [self.trianglesLabel setText:[NSString stringWithFormat:@"Current Triangles: %i, Total Triangles: %i", Get_CurrentNumTriagles(), Get_TotalNumTriangles()]];
}

#pragma mark -
#pragma mark CDEJoystick delegates

- (void)joystick:(CDEJoystick *)aJoystick startMoveAtPoint:(CGPoint)aPoint
{
    std::shared_ptr<CDemoGameTransition> transition = std::static_pointer_cast<CDemoGameTransition>(self.gameTransition);
    
    if(aPoint.x > aJoystick.activeBounds.size.width &&
       aPoint.y > aJoystick.activeBounds.size.height)
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_LEFT);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_BACKWARD);
    }
    else if(aPoint.x > aJoystick.activeBounds.size.width &&
            aPoint.y < aJoystick.activeBounds.origin.y)
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_RIGHT);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_FORWARD);
    }
    else if(aPoint.x < aJoystick.activeBounds.origin.x &&
            aPoint.y > aJoystick.activeBounds.size.height)
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_RIGHT);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_BACKWARD);
    }
    else if(aPoint.x < aJoystick.activeBounds.origin.x &&
            aPoint.y < aJoystick.activeBounds.origin.y)
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_LEFT);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_FORWARD);
    }
    else if(aPoint.x > aJoystick.activeBounds.size.width)
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_RIGHT);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_NONE);
    }
    else if(aPoint.x < aJoystick.activeBounds.origin.x)
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_LEFT);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_NONE);
    }
    else if(aPoint.y > aJoystick.activeBounds.size.height)
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_NONE);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_BACKWARD);
    }
    else if(aPoint.y < aJoystick.activeBounds.origin.y)
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_NONE);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_FORWARD);
    }
    else
    {
        transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_NONE);
        transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_NONE);
    }
}

- (void)joystick:(CDEJoystick *)aJoystick endMoveAtPoint:(CGPoint)aPoint
{
    std::shared_ptr<CDemoGameTransition> transition = std::static_pointer_cast<CDemoGameTransition>(self.gameTransition);
    transition->getUIToSceneCommands()->executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE_NONE);
    transition->getUIToSceneCommands()->executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE_NONE);
}

@end

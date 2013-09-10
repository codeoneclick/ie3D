//
//  CGameViewController.m
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameViewController.h"
#include "COGLWindow_iOS.h"
#include "CKOTHWorkflow.h"
#include "CKOTHInGameTransition.h"
#include "CGameLoopExecutor.h"

@interface CGameViewController ()

@property (weak, nonatomic) IBOutlet COGLWindow_iOS *m_glWindow;
@property (unsafe_unretained, nonatomic) std::string result;
@property (weak, nonatomic) IBOutlet UILabel *fpsLabel;
@property (weak, nonatomic) IBOutlet UILabel *trianglesLabel;
@end

@implementation CGameViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self.view setFrame:CGRectMake(0.0f, 0.0f, [[UIScreen mainScreen] bounds].size.height, [[UIScreen mainScreen] bounds].size.width)];
    
    NSMethodSignature* signature = [self methodSignatureForSelector:@selector(onTick:)];
    NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
    [invocation setTarget: self];
    [invocation setSelector:@selector(onTick:)];
    
    NSTimer *sender = [NSTimer timerWithTimeInterval:0.1 invocation:invocation repeats:YES];
    NSRunLoop *runner = [NSRunLoop currentRunLoop];
    [runner addTimer:sender forMode:NSDefaultRunLoopMode];
    
    CKOTHWorkflow* workflow = new CKOTHWorkflow();
    std::shared_ptr<IGameTransition> transition = workflow->CreateXcomInGameTransition("main.transition.xml", (__bridge void*)self.m_glWindow);
    workflow->RegisterTransition(transition);
    workflow->GoToTransition("main.transition.xml");
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

@end

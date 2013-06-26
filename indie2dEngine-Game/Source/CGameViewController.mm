//
//  CGameViewController.m
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameViewController.h"
#include "COGLWindow_iOS.h"
#include "CGameXcomWorkflow.h"
#include "CGameXcomInGameTransition.h"

@interface CGameViewController ()

@property (weak, nonatomic) IBOutlet COGLWindow_iOS *m_glWindow;
@property (unsafe_unretained, nonatomic) std::string result;
@end

@implementation CGameViewController

class C
{
public:
    int a;
};

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
        
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    CGameXcomWorkflow* workflow = new CGameXcomWorkflow();
    std::shared_ptr<IGameTransition> transition = workflow->CreateXcomInGameTransition("main.transition.xml", (__bridge void*)self.m_glWindow);
    workflow->RegisterTransition(transition);
    workflow->GoToTransition("main.transition.xml");
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

@end

//
//  CGameViewController.m
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameViewController.h"
#include "COGLWindow_iOS.h"
#include "CGame.h"

@interface CGameViewController ()

@property (weak, nonatomic) IBOutlet COGLWindow_iOS *m_glWindow;
@end

@implementation CGameViewController

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
    
    CGame* game = new CGame();
    game->Call_Test_01();
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

@end

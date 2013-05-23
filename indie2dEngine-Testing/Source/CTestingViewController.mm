//
//  CTestingViewController.m
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTestingViewController.h"
#include "COGLWindow_iOS.h"
#include "CRoot.h"
#include "gtest/gtest.h"

@interface CTestingViewController ()

@property (weak, nonatomic) IBOutlet COGLWindow_iOS *m_glWindow;
@property (unsafe_unretained, nonatomic) CRoot* m_root;

@end

@implementation CTestingViewController

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
    self.m_root = new CRoot((__bridge void*)_m_glWindow);
    std::cout<<"[Testing] "<<std::endl<<RUN_ALL_TESTS()<<std::endl;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

@end

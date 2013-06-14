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
    
    
    std::function<void(int)> function1 = [](int a)
    {
        std::cout<<"call"<<a<<std::endl;
    };
    
    std::function<void(std::string)> function2 = [](std::string a)
    {
        std::cout<<"call"<<a<<std::endl;
    };
    
    C* param = new C();
    param->a = 99;
    
    std::function<void(C*)> function3 = [](C* a)
    {
        std::cout<<"call"<<a->a<<std::endl;
    };
    
    std::shared_ptr<C> param2 = std::make_shared<C>();
    param->a = 101;
    
    std::function<void(std::shared_ptr<C> )> function4 = [](std::shared_ptr<C> a)
    {
        std::cout<<"call"<<a->a<<std::endl;
    };

    thread_concurrency_dispatch<int>(get_thread_concurrency_queue(THREAD_CONCURRENCY_QUEUE_PRIORITY_LOW), function1, 1);
    thread_concurrency_dispatch<std::string>(get_thread_concurrency_queue(THREAD_CONCURRENCY_QUEUE_PRIORITY_LOW), function2, "two");
    thread_concurrency_dispatch<C*>(get_thread_concurrency_queue(THREAD_CONCURRENCY_QUEUE_PRIORITY_LOW), function3, param);
    thread_concurrency_dispatch<std::shared_ptr<C>>(get_thread_concurrency_queue(THREAD_CONCURRENCY_QUEUE_PRIORITY_LOW), function4, param2);
    
    //CGameXcomWorkflow* workflow = new CGameXcomWorkflow();
    //std::shared_ptr<IGameTransition> transition = workflow->CreateXcomInGameTransition("main.transition.xml", (__bridge void*)self.m_glWindow);
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

@end

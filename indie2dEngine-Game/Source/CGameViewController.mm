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
#include "thread_concurrency.h"
#include "thread_concurrency_queue.h"

@interface CGameViewController ()

@property (weak, nonatomic) IBOutlet COGLWindow_iOS *m_glWindow;
@property (unsafe_unretained, nonatomic) std::string result;
@property (unsafe_unretained, nonatomic) thread_concurrency concurrency;
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
    
    self.concurrency.dispatch<void, int>(self.concurrency.get_thread_concurrency_queue(THREAD_CONCURRENCY_QUEUE_PRIORITY_LOW), function1, 2);
    self.concurrency.dispatch<void, std::string>(self.concurrency.get_thread_concurrency_queue(THREAD_CONCURRENCY_QUEUE_PRIORITY_LOW), function2, "two");
    self.concurrency.dispatch<void, C*>(self.concurrency.get_thread_concurrency_queue(THREAD_CONCURRENCY_QUEUE_PRIORITY_LOW), function3, param);

    
    //execute<void, int>(queue, function, 5);
    
    //CGameXcomWorkflow* workflow = new CGameXcomWorkflow();
    //std::shared_ptr<IGameTransition> transition = workflow->CreateXcomInGameTransition("main.transition.xml", (__bridge void*)self.m_glWindow);
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    
    
    
    //dispatch_queue_t exampleQueue;
    //exampleQueue = dispatch_queue_create( "com.example.unique.identifier", NULL );
    //DISPATCH_QUEUE_PRIORITY_HIGH
    //dispatch_async(dispatch_get_global_queue(0, 0), ^{

    //    dispatch_async(dispatch_get_main_queue(), ^{

    //    });
    //});
}

@end

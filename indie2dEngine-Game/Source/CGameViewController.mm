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

template<typename RESULT, typename ...ARGS>
void execute(thread_concurrency_queue _queue, std::function<RESULT(ARGS... args)> _function, ARGS... args)
{
    _function(std::forward<ARGS>(args)...);
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    thread_concurrency concurrency;
    thread_concurrency_queue queue("main");
    
    std::function<void(int)> function = [](int a)
    {
        std::cout<<"call"<<a<<std::endl;
    };
    
    execute<void, int>(queue, function, 5);
    
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

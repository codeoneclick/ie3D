//
//  AppDelegate.m
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/3/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "AppDelegate.h"
#include "CGameViewController.h"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    CGameViewController *vc = [CGameViewController new];
    UINavigationController* navigationViewController = [[UINavigationController alloc] initWithRootViewController:vc];
    [navigationViewController setNavigationBarHidden:YES animated:NO];
    self.window.backgroundColor = [UIColor blackColor];
    [self.window setRootViewController:navigationViewController];
    [self.window makeKeyAndVisible];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{

}

- (void)applicationDidEnterBackground:(UIApplication *)application
{

}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
  
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{

}

- (void)applicationWillTerminate:(UIApplication *)application
{
   
}

@end

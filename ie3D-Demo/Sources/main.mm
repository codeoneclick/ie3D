//
//  main.m
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/3/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#if defined(__IOS__)

#import <UIKit/UIKit.h>
#import "AppDelegate-iOS.h"

int main(int argc, char *argv[])
{
    @autoreleasepool
    {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegateIOS class]));
    }
}

#elif defined(__OSX__)

#import <Cocoa/Cocoa.h>

int main(int argc, const char * argv[])
{
    return NSApplicationMain(argc, argv);
}

#endif
//
//  main.m
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/3/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#if defined(__IOS__)

#include <UIKit/UIKit.h>
#include "AppDelegate-iOS.h"
#include "HCommon.h"

int main(int argc, char *argv[])
{
    @autoreleasepool
    {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegateIOS class]));
    }
}

#elif defined(__OSX__)

#include <Cocoa/Cocoa.h>
#include "HCommon.h"

int main(int argc, const char * argv[])
{
    return NSApplicationMain(argc, argv);
}

#endif
//
//  main.m
//  indie2dEngine-Testing
//
//  Created by Sergey Sergeev on 5/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include <UIKit/UIKit.h>
#include "AppDelegate.h"
#include "gtest/gtest.h"

int main(int argc, char *argv[])
{
    @autoreleasepool {
        ::testing::InitGoogleTest(&argc, argv);
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}

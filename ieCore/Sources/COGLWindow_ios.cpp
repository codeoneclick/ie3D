//
//  IOGLWindow.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IOGLWindow.h"

#if defined(__IOS__)

@implementation IOGLWindow

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame;
{
    if (self = [super initWithFrame:frame])
    {
        super.layer.opaque = YES;
    }
    return self;
}

@end

#endif
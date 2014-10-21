//
//  AppDelegateOSX.mm
//  view-mac
//
//  Created by sergey.sergeev on 9/19/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#include "AppDelegate-OSX.h"
#include "CDemoController-OSX.h"
#include "IOGLWindow.h"

@interface AppDelegateOSX ()

@property (weak) IBOutlet NSWindow *window;
@property (weak) IBOutlet NSView *openGLView;
@property (nonatomic, unsafe_unretained) std::shared_ptr<CDemoControllerOSX> demoController;

@end

@implementation AppDelegateOSX

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    OpenGLView *customView = [[OpenGLView alloc] initWithFrame:CGRectMake(0.0,
                                                                          0.0,
                                                                          self.openGLView.frame.size.width,
                                                                          self.openGLView.frame.size.height)];
    [self.openGLView addSubview:customView];
    self.demoController = std::make_shared<CDemoControllerOSX>(customView);
    self.demoController->create();
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
    
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)aSender
{
    return YES;
}

@end
//
//  main.m
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/3/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

//#include "SDL.h"
//#include "SDL_opengles.h"
//#include "CKOTHWorkflow.h"


int main(int argc, char *argv[])
{
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}

/*
static int screen_w, screen_h;
#define MILLESECONDS_PER_FRAME 16

int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_GLContext context;
    int w, h;
    Uint32 startFrame;
    Uint32 endFrame;
    Uint32 delay;
    int done;
    

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_RETAINED_BACKING, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    

    window = SDL_CreateWindow(NULL, 0, 0, 640, 480,
                              SDL_WINDOW_OPENGL |
                              SDL_WINDOW_BORDERLESS);
    context = SDL_GL_CreateContext(window);
    
    
    
    SDL_GetWindowSize(window, &screen_w, &screen_h);
    
    CKOTHWorkflow* workflow = new CKOTHWorkflow();
    std::shared_ptr<IGameTransition> transition = workflow->CreateXcomInGameTransition("main.transition.xml", nullptr);
    workflow->RegisterTransition(transition);
    workflow->GoToTransition("main.transition.xml");
    
    glViewport(0, 0, screen_w, screen_h);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    
    done = 0;
    while (!done) {
        startFrame = SDL_GetTicks();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = 1;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
            }
        }
       
        SDL_GL_SwapWindow(window);
        endFrame = SDL_GetTicks();
        

        delay = MILLESECONDS_PER_FRAME - (endFrame - startFrame);
        if (delay > MILLESECONDS_PER_FRAME) {
            delay = MILLESECONDS_PER_FRAME;
        }
        if (delay > 0) {
            SDL_Delay(delay);
        }
    }
    
    SDL_Quit();
    
    return 0;
}
*/
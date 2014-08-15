//
//  CDEJoystick.m
//  ie3D-Demo
//
//  Created by sergey.sergeev on 8/15/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#import "CDEJoystick.h"

@interface CDEJoystick()

@property (nonatomic, strong) UIImageView *backgroundImageView;
@property (nonatomic, strong) UIImageView *controlImageView;

@end

@implementation CDEJoystick

- (instancetype)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if(self)
    {
        self.activeBounds = CGRectMake(32.0,
                                       32.0,
                                       self.frame.size.width - 32.0,
                                       self.frame.size.height - 32.0);
        
        self.backgroundImageView = [[UIImageView alloc] initWithFrame:CGRectMake(0.0,
                                                                                 0.0,
                                                                                 self.frame.size.width,
                                                                                 self.frame.size.height)];
        [self.backgroundImageView setImage:[UIImage imageNamed:@"navigation_joystick"]];
        [self.backgroundImageView setBackgroundColor:[UIColor clearColor]];
        [self.backgroundImageView setAlpha:0.25];
        [self addSubview:self.backgroundImageView];
        
        self.controlImageView = [[UIImageView alloc] initWithFrame:CGRectMake(self.frame.size.width / 2.0 - (self.frame.size.width / 3.0) / 2.0,
                                                                              self.frame.size.height / 2.0 - (self.frame.size.height / 3.0) / 2.0,
                                                                              self.frame.size.width / 3.0,
                                                                              self.frame.size.height / 3.0)];
        [self.controlImageView setImage:[UIImage imageNamed:@"navigation_joystick"]];
        [self.controlImageView setBackgroundColor:[UIColor clearColor]];
        [self.controlImageView setAlpha:0.5];
        [self addSubview:self.controlImageView];
    }
    return self;
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    for (UITouch* touch in touches)
    {
        CGPoint point = [touch locationInView:self];
        [self update:point];
    }
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    for (UITouch* touch in touches)
    {
        CGPoint point = [touch locationInView:self];
        [self update:point];
    }
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    if([self.delegate respondsToSelector:@selector(joystick:endMoveAtPoint:)])
    {
        [self.delegate joystick:self endMoveAtPoint:CGPointZero];
    }
    
    CGRect frame = self.controlImageView.frame;
    frame.origin.x = self.frame.size.width / 2.0 - (self.frame.size.width / 3.0) / 2.0;
    frame.origin.y = self.frame.size.height / 2.0 - (self.frame.size.height / 3.0) / 2.0;
    self.controlImageView.frame = frame;
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
    if([self.delegate respondsToSelector:@selector(joystick:endMoveAtPoint:)])
    {
        [self.delegate joystick:self endMoveAtPoint:CGPointZero];
    }
    
    CGRect frame = self.controlImageView.frame;
    frame.origin.x = self.frame.size.width / 2.0 - (self.frame.size.width / 3.0) / 2.0;
    frame.origin.y = self.frame.size.height / 2.0 - (self.frame.size.height / 3.0) / 2.0;
    self.controlImageView.frame = frame;
}

- (void)update:(CGPoint)point
{
    if([self.delegate respondsToSelector:@selector(joystick:startMoveAtPoint:)])
    {
        [self.delegate joystick:self startMoveAtPoint:point];
    }
    
    CGRect frame = self.controlImageView.frame;
    frame.origin.x = point.x - frame.size.width / 2;
    frame.origin.y = point.y - frame.size.height / 2;
    self.controlImageView.frame = frame;
}

@end

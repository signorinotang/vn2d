//
//  vnGLESViewController.mm
//  vn2d
//
//  Created by Wayne on 14-1-10.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#import "vnGLESViewController.h"
#include "vnApplicationImpl.h"

@implementation GLESViewController

- (id)initWithAppImpl:(_vn_ns::ApplicationImpl *)impl
{
    m_impl = impl;
    if ([self init]) {
        self.delegate = self;
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(onKeyboardWillShow:)
                                                     name:UIKeyboardWillShowNotification object:nil];
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(onKeyboardDidShow:)
                                                     name:UIKeyboardDidShowNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(onKeyboardWillHide:)
                                                     name:UIKeyboardWillHideNotification object:nil];
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(onKeyboardDidHide:)
                                                     name:UIKeyboardDidHideNotification object:nil];
    }
    return self;
}

#pragma mark -

- (void)glkViewControllerUpdate:(GLKViewController *)controller
{
    m_impl->_view_update(self.timeSinceLastUpdate);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    m_impl->_view_render();
}

- (void)glkViewController:(GLKViewController *)controller willPause:(BOOL)pause
{
    if (pause) {
        m_impl->_view_suspend();
    } else {
        m_impl->_view_resume();
    }
}

/////////////////////////////////////
// Keyboard Notification

- (void)onKeyboardWillShow:(NSNotification *)notif
{
	m_impl->_keyboard_willShow();
}

- (void)onKeyboardDidShow:(NSNotification *)notif
{
	m_impl->_keyboard_didShow();
}

- (void)onKeyboardWillHide:(NSNotification *)notif
{
	m_impl->_keyboard_willHide();
}

- (void)onKeyboardDidHide:(NSNotification *)notif
{
	m_impl->_keyboard_didHide();
}

#pragma mark -

@end
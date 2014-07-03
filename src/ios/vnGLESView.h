//
//  vnGLESView.h
//  vn2d
//
//  Created by Wayne on 14-1-10.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __vn2d__vnGLESView__
#define __vn2d__vnGLESView__

#import <GLKit/GLKit.h>
#include "vnTypes.h"

_vn_begin
class ApplicationImpl;
_vn_end


@interface GLESView : GLKView<UITextInput> {
    _vn_ns::ApplicationImpl *m_impl;
    _vn_ns::str32 m_markedText;
}

- (id)initWithFrame:(CGRect)frame appImpl:(_vn_ns::ApplicationImpl *)impl;
- (void)setInputAsciiOnly:(bool)flag;
- (bool)isInputAsciiOnly;

// UITextInput
@property(nonatomic, readonly) UITextPosition *beginningOfDocument;
@property(nonatomic, readonly) UITextPosition *endOfDocument;
@property(nonatomic, assign) id<UITextInputDelegate> inputDelegate;
@property(nonatomic, readonly) UITextRange *markedTextRange;
@property (nonatomic, copy) NSDictionary *markedTextStyle;
@property(readwrite, copy) UITextRange *selectedTextRange;
@property(nonatomic, readonly) id<UITextInputTokenizer> tokenizer;
@property(nonatomic) UIKeyboardType keyboardType;
@property(nonatomic) UIReturnKeyType returnKeyType;

@end


// GLESViewTextPosition
@interface GLESViewTextPosition : UITextPosition {
    
}

@end

// GLESViewTextRange
@interface GLESViewTextRange : UITextRange {
	
}

@property (nonatomic, readonly) GLESViewTextPosition *start;
@property (nonatomic, readonly) GLESViewTextPosition *end;

+ (GLESViewTextRange *) sharedInstance;

@end

#endif /* defined(__vn2d__vnGLESView__) */

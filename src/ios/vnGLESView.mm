//
//  vnGLESView.mm
//  vn2d
//
//  Created by Wayne on 14-1-10.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#import "vnGLESView.h"
#include "vnApplicationImpl.h"

#pragma mark -

@implementation GLESView

- (id)initWithFrame:(CGRect)frame appImpl:(_vn_ns::ApplicationImpl *)impl
{
    m_impl = impl;
    EAGLContext *context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    [EAGLContext setCurrentContext:context];
    self = [self initWithFrame:frame context:context];
    if (self) {
        returnKeyType = UIReturnKeyDone;
        keyboardType = UIKeyboardTypeDefault;
        [self setMultipleTouchEnabled:YES];
    }
    return self;
}

- (void)setInputAsciiOnly:(bool)flag
{
    keyboardType = (flag ? UIKeyboardTypeASCIICapable : UIKeyboardTypeDefault);
}

- (bool)isInputAsciiOnly
{
    return keyboardType == UIKeyboardTypeASCIICapable;
}

#pragma mark -
#pragma mark Handle Touches

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch *touch in touches) {
		CGPoint pt = [touch locationInView:self];
		m_impl->_view_touchBegan((long)touch, _vn_ns::vector2i(pt.x, pt.y));
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch *touch in touches) {
		CGPoint pt = [touch locationInView:self];
		m_impl->_view_touchMoved((long)touch, _vn_ns::vector2i(pt.x, pt.y));
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch *touch in touches) {
		CGPoint pt = [touch locationInView:self];
		m_impl->_view_touchEnded((long)touch, _vn_ns::vector2i(pt.x, pt.y));
	}
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch *touch in touches) {
		CGPoint pt = [touch locationInView:self];
		m_impl->_view_touchCancelled((long)touch, _vn_ns::vector2i(pt.x, pt.y));
	}
}

#pragma mark -
#pragma mark UITextInput
// UIKeyInput
- (BOOL)canBecomeFirstResponder
{
	return YES;
}

- (void)insertText:(NSString *)text
{
	m_impl->_text_insert((const _vn_ns::c32 *)[text cStringUsingEncoding:NSUTF32StringEncoding]);
}

- (void)deleteBackward
{
	m_impl->_text_deleteBackward();
}

- (BOOL)hasText
{
	return NO;
}
// UITextInput
@synthesize beginningOfDocument;
@synthesize endOfDocument;
@synthesize inputDelegate;
@synthesize markedTextRange;
@synthesize markedTextStyle;
@synthesize tokenizer;
@synthesize keyboardType;
@synthesize returnKeyType;

- (NSString *)textInRange:(UITextRange *)range
{
	return @"";
}

- (void)replaceRange:(UITextRange *)range withText:(NSString *)text
{
	
}

-(UITextRange *)selectedTextRange
{
	return nil;
}

- (void)setSelectedTextRange:(UITextRange *)textRange
{
	
}

- (void)setMarkedTextRange:(UITextRange *)markedTextRange
{
    
}

- (UITextRange *)markedTextRange
{
    return [GLESViewTextRange sharedInstance];
}

- (void)setMarkedTextStyle:(NSDictionary *)markedTextStyle
{
    
}

- (NSDictionary *)markedTextStyle
{
    return nil;
}

- (void)setMarkedText:(NSString *)markedText selectedRange:(NSRange)selectedRange
{
    _vn_ns::str32 str = (const _vn_ns::c32 *)[markedText cStringUsingEncoding:NSUTF32StringEncoding];
    if (str != m_markedText) {
        m_markedText = str;
        if (!m_markedText.empty()) {
            m_impl->_text_mark(m_markedText);
        }
    }
    
}

- (void)unmarkText
{
    if (!m_markedText.empty()) {
      	m_impl->_text_unmark();
        m_impl->_text_insert(m_markedText);
        m_markedText.clear();
    }
}

- (UITextRange *)textRangeFromPosition:(UITextPosition *)fromPosition toPosition:(UITextPosition *)toPosition
{
    return nil;
}

- (UITextPosition *)positionFromPosition:(UITextPosition *)position offset:(NSInteger)offset
{
    return nil;
}

- (UITextPosition *)positionFromPosition:(UITextPosition *)position inDirection:(UITextLayoutDirection)direction offset:(NSInteger)offset
{
    return nil;
}

- (NSComparisonResult)comparePosition:(UITextPosition *)position toPosition:(UITextPosition *)other
{
    return NSOrderedSame;
}

- (NSInteger)offsetFromPosition:(UITextPosition *)from toPosition:(UITextPosition *)toPosition
{
    return 0;
}

- (UITextPosition *)positionWithinRange:(UITextRange *)range farthestInDirection:(UITextLayoutDirection)direction
{
    return nil;
}

- (UITextRange *)characterRangeByExtendingPosition:(UITextPosition *)position inDirection:(UITextLayoutDirection)direction
{
    return nil;
}

- (UITextWritingDirection)baseWritingDirectionForPosition:(UITextPosition *)position inDirection:(UITextStorageDirection)direction
{
    return UITextWritingDirectionNatural;
}

- (void)setBaseWritingDirection:(UITextWritingDirection)writingDirection forRange:(UITextRange *)range
{
    
}

- (CGRect)firstRectForRange:(UITextRange *)range
{
    return CGRectZero;
}

- (CGRect)caretRectForPosition:(UITextPosition *)position
{
    return CGRectZero;
}

- (UITextPosition *)closestPositionToPoint:(CGPoint)point
{
    return nil;
}

- (UITextPosition *)closestPositionToPoint:(CGPoint)point withinRange:(UITextRange *)range
{
    return nil;
}

- (UITextRange *)characterRangeAtPoint:(CGPoint)point
{
    return nil;
}

- (NSArray *)selectionRectsForRange:(UITextRange *)range
{
	return nil;
}

@end

#pragma mark -

//////////////////////////////////////////
@implementation GLESViewTextPosition

@end

@implementation GLESViewTextRange

-(id)init
{
    self = [super init];
    if (self) {
        start = [[GLESViewTextPosition alloc] init];
        end = [[GLESViewTextPosition alloc] init];
    }
    return self;
}

- (BOOL)isEmpty
{
    return YES;
}

@synthesize start;
@synthesize end;

+ (GLESViewTextRange *) sharedInstance
{
    static GLESViewTextRange *s_ins = [[GLESViewTextRange alloc] init];
    return s_ins;
}

@end

//
//  YYAsyncLayer.m
//  yyStudy
//
//  Created by hanxu on 2017/4/24.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import "YYAsyncLayer.h"
#import "YYSentinel.h"
#import <libkern/OSAtomic.h>
static dispatch_queue_t YYAsyncLayerGetReleaseQueue()
{
    return dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0);
}

static dispatch_queue_t YYAsyncLayerGetDisplayQueue()
{
#define MAX_QUEUE_COUNT 16
    static int queueCount;
    static dispatch_queue_t queues[MAX_QUEUE_COUNT];
    static int32_t counter = 0;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        queueCount = (int)[NSProcessInfo processInfo].activeProcessorCount;
        queueCount = queueCount < 1 ? 1 : queueCount > MAX_QUEUE_COUNT ? MAX_QUEUE_COUNT : queueCount;
        if ([UIDevice currentDevice].systemVersion.floatValue >= 8.0) {
            for (NSUInteger i = 0; i < queueCount; i++) {
                dispatch_queue_attr_t attr = dispatch_queue_attr_make_with_qos_class(DISPATCH_QUEUE_SERIAL, QOS_CLASS_USER_INITIATED, 0);
                queues[i] = dispatch_queue_create("com.yy.render", attr);
            }
        } else {
            for (NSUInteger i = 0; i < queueCount; i++) {
                queues[i] = dispatch_queue_create("com.yy.render", DISPATCH_QUEUE_SERIAL);
                dispatch_set_target_queue(queues[i], dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0));
            }
        }
        
    });
    int32_t cur = OSAtomicIncrement32(&counter);
    if (cur < 0) {
        cur = -cur;
    }
    return queues[(cur) % queueCount];
}

@implementation YYAsyncLayerDisplayTask

@end

@implementation YYAsyncLayer
{
    YYSentinel *_sentinel;
    CGFloat _yyScale;
}

#pragma mark - 重写
+ (id)defaultValueForKey:(NSString *)key
{
    if ([key isEqualToString:@"displaysAsynchronously"]) {
        return @YES;
    } else {
        return [super defaultValueForKey:key];
    }
}

- (instancetype)init
{
    if (self = [super init]) {
        static CGFloat scale;
        static dispatch_once_t onceToken;
        dispatch_once(&onceToken, ^{
            scale = [UIScreen mainScreen].scale;
        });
        _yyScale = scale;
        _sentinel = [YYSentinel new];
        _displaysAsynchronously = YES;
        [NSTimer scheduledTimerWithTimeInterval:0.5 repeats:YES block:^(NSTimer * _Nonnull timer) {
            NSLog(@"%f",self.contentsScale);
        }];
    }
    return self;
}

- (void)dealloc
{
    [_sentinel increase];
}

- (void)setNeedsDisplay
{
    //需要重绘,把计数器加一,取消前一次重绘
    [self _cancelAsyncDisplay];
    [super setNeedsDisplay];
}

- (void)display
{
    super.contents = super.contents;
    [self _displayAsync:_displaysAsynchronously];
}

#pragma mark - 私有
- (void)_displayAsync:(BOOL)async
{
    __strong id<YYAsyncLayerDelegate> delegate = (id)self.delegate;
    YYAsyncLayerDisplayTask *task = [delegate newAsyncDisplayTask];
    if (!task.display) {
        if (task.willDisplay) {
            task.willDisplay(self);
        }
        self.contents = nil;
        if (task.didDisplay) {
            task.didDisplay(self, YES);
        }
        return;
    }
    
    if (async) {
        if (task.willDisplay) {
            task.willDisplay(self);
        }
        YYSentinel *sentinel = _sentinel;
        int32_t value = sentinel.value;
        BOOL (^isCancelled)() = ^BOOL() {
            return value != sentinel.value;
        };
        
        CGSize size = self.bounds.size;
        BOOL opaque = self.opaque;
        
        CGColorRef backgroundColor = (opaque && self.backgroundColor) ? CGColorRetain(self.backgroundColor) : NULL;
        if (size.width < 1 || size.height < 1) {
            CGImageRef image = (__bridge_retained CGImageRef)(self.contents);
            self.contents = nil;
            if (image) {
                dispatch_async(YYAsyncLayerGetReleaseQueue(), ^{
                    CFRelease(image);
                });
            }
            if (task.didDisplay) {
                task.didDisplay(self, YES);
            }
            CGColorRelease(backgroundColor);
            return;
        }
        
        dispatch_async(YYAsyncLayerGetDisplayQueue(), ^{
            if (isCancelled()) {
                CGColorRelease(backgroundColor);
                return ;
            }
            UIGraphicsBeginImageContextWithOptions(size, opaque, _yyScale);
            CGContextRef context = UIGraphicsGetCurrentContext();
            if (opaque) {
                CGContextSaveGState(context);
                
                {
                    //如果背景色为nil,或者alpha小于1
                    if (!backgroundColor || CGColorGetAlpha(backgroundColor) < 1) {
                        CGContextSetFillColorWithColor(context, [UIColor whiteColor].CGColor);
                        CGContextAddRect(context, CGRectMake(0, 0, size.width * _yyScale, size.height * _yyScale));
                        CGContextFillPath(context);
                    }
                    
                    if (backgroundColor) {
                        CGContextSetFillColorWithColor(context, backgroundColor);
                        CGContextAddRect(context, CGRectMake(0, 0, size.width * _yyScale, size.height * _yyScale));
                        CGContextFillPath(context);
                    }
                }
                CGContextRestoreGState(context);
                CGColorRelease(backgroundColor);
            }
            
            task.display(context, size, isCancelled);
            if (isCancelled()) {
                UIGraphicsEndImageContext();
                dispatch_async(dispatch_get_main_queue(), ^{
                    if (task.didDisplay) {
                        task.didDisplay(self, NO);
                    }
                });
            }
            UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
            UIGraphicsEndImageContext();
            if (isCancelled()) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    if (task.didDisplay) {
                        task.didDisplay(self, NO);
                    }
                });
                return;
            }
            
            dispatch_async(dispatch_get_main_queue(), ^{
                if (isCancelled()) {
                    if (task.didDisplay) {
                        task.didDisplay(self, NO);
                    }
                } else {
                    self.contents = (__bridge id)(image.CGImage);
                    if (task.didDisplay) {
                        task.didDisplay(self, YES);
                    }
                }
            });
        });
    } else {
        [_sentinel increase];
        if (task.willDisplay) {
            task.willDisplay(self);
        }
        UIGraphicsBeginImageContextWithOptions(self.bounds.size, self.opaque, _yyScale);
        CGContextRef context = UIGraphicsGetCurrentContext();;
        if (self.opaque) {
            CGSize size = self.bounds.size;
            size.width *= _yyScale;
            size.height *= _yyScale;
            CGContextSaveGState(context);
            
            {
                if (!self.backgroundColor || CGColorGetAlpha(self.backgroundColor) < 1) {
                    CGContextSetFillColorWithColor(context, [UIColor whiteColor].CGColor);
                    CGContextAddRect(context, CGRectMake(0, 0, size.width, size.height));
                    CGContextFillPath(context);
                }
                if (self.backgroundColor) {
                    CGContextSetFillColorWithColor(context, self.backgroundColor);
                    CGContextAddRect(context, CGRectMake(0, 0, size.width, size.height));
                    CGContextFillPath(context);
                }
            }
            
            CGContextRestoreGState(context);
        }
        
        task.display(context, self.bounds.size, ^{return NO;});
        UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();
        self.contents = (__bridge id)(image.CGImage);
        if (task.didDisplay) {
            task.didDisplay(self, YES);
        }
        
    }
}

- (void)_cancelAsyncDisplay
{
    [_sentinel increase];
}

@end

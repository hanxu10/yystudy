//
//  _YYWebImageSetter.m
//  yyStudy
//
//  Created by hanxu on 2017/5/3.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import "_YYWebImageSetter.h"
#import <libkern/OSAtomic.h>


NSString *const _YYWebImageFadeAnimationKey = @"YYWebImageFade";
const NSTimeInterval _YYWebImageFadeTime = 0.2;
const NSTimeInterval _YYWebImageProgressiveFadeTime = 0.4;


@implementation _YYWebImageSetter
{
    dispatch_semaphore_t _lock;
    NSURL *_imageURL;
    NSOperation *_operation;
    int32_t _sentinel;
}

- (instancetype)init
{
    self = [super init];
    _lock = dispatch_semaphore_create(1);
    return self;
}

- (NSURL *)imageURL
{
    dispatch_semaphore_wait(_lock, DISPATCH_TIME_FOREVER);
    NSURL *imageURL = _imageURL;
    dispatch_semaphore_signal(_lock);
    return imageURL;
}

- (void)dealloc
{
    OSAtomicIncrement32(&_sentinel);
    [_operation cancel];
}



@end

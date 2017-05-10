//
//  YYWebImageOperation.m
//  yyStudy
//
//  Created by hanxu on 2017/5/5.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import "YYWebImageOperation.h"


/// Returns nil in App Extension.
static UIApplication *_YYSharedApplication() {
    static BOOL isAppExtension = NO;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        Class cls = NSClassFromString(@"UIApplication");
        if(!cls || ![cls respondsToSelector:@selector(sharedApplication)]) isAppExtension = YES;
        if ([[[NSBundle mainBundle] bundlePath] hasSuffix:@".appex"]) isAppExtension = YES;
    });
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundeclared-selector"
    return isAppExtension ? nil : [UIApplication performSelector:@selector(sharedApplication)];
#pragma clang diagnostic pop
}

///如果右下角的像素被填满，则返回YES。
static BOOL YYCGImageLastPixelFilled(CGImageRef image)
{
    if (!image) {
        return;
    }
    size_t width = CGImageGetWidth(image);
    size_t height = CGImageGetHeight(image);
    if (width == 0 || height == 0) {
        return NO;
    }
    CGContextRef ctx = CGBitmapContextCreate(NULL, 1, 1, 8, 0, YYCGColorSpaceGetDeviceRGB(), kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrderDefault);
    if (!ctx) {
        return NO;
    }
    CGContextDrawImage(ctx, CGRectMake(-width + 1, 0, width, height), image);
    uint8_t *bytes = CGBitmapContextGetData(ctx);
    BOOL isAlpha = bytes && bytes[0];
    CFRelease(ctx);
    return !isAlpha;
}

@implementation YYWebImageOperation

@end

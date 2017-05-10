//
//  YYImageCoder.m
//  yyStudy
//
//  Created by hanxu on 2017/5/5.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import "YYImageCoder.h"
#import <QuartzCore/QuartzCore.h>


#pragma mark - Helper




CGColorSpaceRef YYCGColorSpaceGetDeviceRGB()
{
    static CGColorSpaceRef space;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        space = CGColorSpaceCreateDeviceRGB();
    });
    return space;
}










@implementation YYImageCoder

@end

//
//  YYSentinel.m
//  yyStudy
//
//  Created by hanxu on 2017/4/24.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import "YYSentinel.h"
#import <libkern/OSAtomic.h>

@implementation YYSentinel
{
    int32_t _value;
}

- (int32_t)value
{
    return _value;
}


- (int32_t)increase
{
    return OSAtomicIncrement32(&_value);
}
@end

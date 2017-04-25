//
//  YYSentinel.h
//  yyStudy
//
//  Created by hanxu on 2017/4/24.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import <Foundation/Foundation.h>


NS_ASSUME_NONNULL_BEGIN

/**
 *  Sentinel是一个线程安全递增计数器。 它可能在某些多线程情况下使用。
 *
 *
 */

@interface YYSentinel : NSObject

///返回计数器的当前值。
@property (readonly) int32_t value;


///以原子方式增加值。
/// @return新值。
- (int32_t)increase;

@end


NS_ASSUME_NONNULL_END

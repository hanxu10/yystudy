//
//  YYTransaction.h
//  yyStudy
//
//  Created by hanxu on 2017/4/24.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

//YYTransaction使我们可以在当前runloop睡眠之前执行一个selector
@interface YYTransaction : NSObject

/**
 *   使用target和selector创建并返回一个transaction
 *  @param target   指定的target,这个target被retained,直到runloop结束
 *  @param selector 为target指定的selector
 */
+ (YYTransaction *)transactionWithTarget:(id)target selector:(SEL)selector;

/**
 *  提交这个transaction到main runloop
 *  @discussion 将执行这个selector,在main runloop的当前loop睡眠之前.如果相同的transaction(相同的target和
 *  selector)已经被提交到这一次loop,就啥也不干
 */
- (void)commit;


@end

NS_ASSUME_NONNULL_END

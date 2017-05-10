//
//  _YYWebImageSetter.h
//  yyStudy
//
//  Created by hanxu on 2017/5/3.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "YYWebImageManager.h"
#import <pthread.h>
NS_ASSUME_NONNULL_BEGIN

/**
 在主队列提交一个block,并等它执行完.
 */
static inline void _yy_dispatch_sync_on_main_queue(void (^block)()) {
    if (pthread_main_np()) {
        block();
    } else {
        dispatch_sync(dispatch_get_main_queue(), block);
    }
}

extern NSString *const _YYWebImageFadeAnimationKey;
extern const NSTimeInterval _YYWebImageFadeTime;
extern const NSTimeInterval _YYWebImageProgressiveFadeTime;


@interface _YYWebImageSetter : NSObject
@property (nonatomic, nullable, readonly) NSURL *imageURL;//当前图片url
@property (nonatomic, readonly) int32_t sentinel;//当前哨兵


//为web image 创建一个新operation,并返回一个哨兵值
- (int32_t)setOperationWithSentinel:(int32_t)sentinel
                                url:(nullable NSURL *)imageURL
                            options:(YYWebImageOptions)options
                            manager:(YYWebImageManager *)manager
                           progress:(nullable YYWebImageProgressBlock)progress
                          transform:(nullable YYWebImageTransformBlock)transform
                         completion:(nullable YYWebImageCompletionBlock)completion;

//取消并返回一个哨兵值.iamgeURL会被设置为nil
- (int32_t)cancel;

//取消并返回一个哨兵值.imageURL将被设置为一个新的值
- (int32_t)cancelWithNewURL:(nullable NSURL *)imageURL;

//设置operation的队列
+ (dispatch_queue_t)setterQueue;

@end


NS_ASSUME_NONNULL_END

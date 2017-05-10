//
//  YYWebImageOperation.h
//  yyStudy
//
//  Created by hanxu on 2017/5/5.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "YYWebImageManager.h"
@class YYImageCache;

NS_ASSUME_NONNULL_BEGIN

@interface YYWebImageOperation : NSOperation

@property (nonatomic, strong, readonly) NSURLRequest *request;///< The image URL request.
@property (nullable, nonatomic, strong, readonly) NSURLResponse *response;///< The response for request.
@property (nullable, nonatomic, strong, readonly)YYImageCache *cache;///< The image cache.
@property (nonatomic, strong, readonly) NSString *cacheKey; ///< The image cache key.
@property (nonatomic, readonly) YYWebImageOptions options; ///< The operation's option.

/**
   URL连接是否应该咨询证书存储以验证连接。 默认为YES。
  
   @discussion 这是在`NSURLConnectionDelegate`代理方法 `-connectionShouldUseCredentialStorage：`中返回的值。
 */
@property (nonatomic) BOOL shouldUseCredentialStorage;

/**
   在“-connection：didReceiveAuthenticationChallenge：”中用于认证挑战的凭据。
  
   @discussion 这将被任何存在于请求URL的用户名或密码的共享凭据（如果存在）覆盖。
   @discussion This will be overridden by any shared credentials that exist for the
               username or password of the request URL, if present.
 */
@property (nullable, nonatomic, strong) NSURLCredential *credential;

/**
 Creates and returns a new operation.
 
 You should call `start` to execute this operation, or you can add the operation
 to an operation queue.
 
 @param request    The Image request. This value should not be nil.
 @param options    A mask to specify options to use for this operation.
 @param cache      An image cache. Pass nil to avoid image cache.
 @param cacheKey   An image cache key. Pass nil to avoid image cache.
 @param progress   A block invoked in image fetch progress.
 The block will be invoked in background thread. Pass nil to avoid it.
 @param transform  A block invoked before image fetch finished to do additional image process.
 The block will be invoked in background thread. Pass nil to avoid it.
 @param completion A block invoked when image fetch finished or cancelled.
 The block will be invoked in background thread. Pass nil to avoid it.
 
 @return The image request opeartion, or nil if an error occurs.
 
 
 
   创建并返回一个新operation。
  
   您应该调用`start`来执行此操作，或者可以将操作添加到操作队列。
  
   @param 图像请求。 该值不应为nil。
   @param options 指定用于此操作的选项的掩码。
   @param cache 图像缓存。传nil，以避免图像缓存。
   @param cacheKey 一个图像缓存键。 传nil，以避免图像缓存。
   @param progress 在图像提取进度中调用的block。该block将在后台线程中被调用。
 
   @param transform 在图像提取完成之前调用的block,用来执行额外的图像处理。该block将在后台线程中被调用。
   @param completion 当图像提取完成或取消时调用的block。 该block将在后台线程中被调用。
  
   @return 图像请求opeartion，如果发生错误，则为nil。
 */
- (instancetype)initWithRequest:(NSURLRequest *)request
                        options:(YYWebImageOptions)options
                          cache:(nullable YYImageCache *)cache
                       cacheKey:(nullable NSString *)cacheKey
                       progress:(nullable YYWebImageProgressBlock)progress
                      transform:(nullable YYWebImageTransformBlock)transform
                     completion:(nullable YYWebImageCompletionBlock)completion NS_DESIGNATED_INITIALIZER;

- (instancetype)init UNAVAILABLE_ATTRIBUTE;
+ (instancetype)new UNAVAILABLE_ATTRIBUTE;

@end
NS_ASSUME_NONNULL_END

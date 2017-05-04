//
//  YYWebImageManager.h
//  yyStudy
//
//  Created by hanxu on 2017/5/3.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import <Foundation/Foundation.h>
NS_ASSUME_NONNULL_BEGIN

@class YYWebImageOperation;

/// The options to control image operation.
typedef NS_OPTIONS(NSUInteger, YYWebImageOptions) {
    YYWebImageOptionShowNetworkActivity = 1 << 0,// 在状态栏显示 network activity,当下载图片时.
    YYWebImageOptionProgressive = 1 << 1,//显示progressive/interlaced/baseline图片,在下载的时候 (和 web browser 一样).
    YYWebImageOptionProgressiveBlur = 1 << 2,//在下载时,显示模糊的progressive JPEG or interlaced PNG图片.为了更好的用户体验,会忽略baseline图片.
    YYWebImageOptionUseNSURLCache = 1 << 3,//使用NSURLCache而不是YYImageCache.
    YYWebImageOptionAllowInvalidSSLCertificates = 1 << 4,//允许不受信任的SSL证书。
    YYWebImageOptionAllowBackgroundTask = 1 << 5,///允许后台任务,在应用程序在后台时下载图像。
    YYWebImageOptionHandleCookies = 1 << 6,//处理存储在NSHTTPCookieStore中的cookie。
    YYWebImageOptionRefreshImageCache = 1 << 7,//从远程加载图像并刷新图像缓存。
    YYWebImageOptionIgnoreDiskCache = 1 << 8,///不要将图像加载到磁盘缓存,也不要从磁盘缓存获得图像。
    YYWebImageOptionIgnorePlaceHolder = 1 << 9,//在设置新的URL之前，不要更改view的图像。
    YYWebImageOptionIgnoreImageDecoding = 1 << 10,///忽略图像解码。这可以用于不需要显示的图像。
    YYWebImageOptionIgnoreAnimatedImage = 1 << 11,///忽略多帧图像解码。这将处理GIF/APNG/WebP/ICO图像为单帧图像。
    YYWebImageOptionSetImageWithFadeAnimation = 1 << 12,///在给view设置图像时,使用淡入淡出动画。这将在image view的layer上添加一个“淡入”动画，以获得更好的用户体验。
    YYWebImageOptionAvoidSetImage = 1 << 13,///图像提取完成后，请勿将image设给view。您可以手动设置图像。
    YYWebImageOptionIgnoreFailedURL = 1 << 14,///当URL无法下载时，此标志会将URL添加到黑名单（内存中)///所以该库不会继续尝试。
};

//指示这个image来自哪里
typedef NS_ENUM(NSUInteger, YYWebImageFromType) {
    YYWebImageFromNone = 0,
    YYWebImageFromMemoryCacheFast,///立即从内存缓存中获取。如果你调用了“setImageWithURL：...”并且图像已经在内存中，那么你将在同一个调用中获取这个值。
    YYWebImageFromMemoryCache,///从内存缓存中获取
    YYWebImageFromDiskCache,///从磁盘缓存中获取。
    YYWebImageFromRemote,///从远程（web或文件路径）获取。
};

///指示图像提取完成阶段。
typedef NS_ENUM(NSInteger, YYWebImageStage) {
    YYWebImageStageProgress  = -1,/// 未完成, progressive image.
    YYWebImageStageCancelled = 0,/// 取消了
    YYWebImageStageFinished  = 1,/// 完成了 (成功或者失败).
};

/**
 这个block在远程获取过程中调用.
 
 @param receivedSize 当前获得的大小in bytes.
 @param expectedSize 期望的总大小 in bytes (-1 意味着不知道大小).
 */
typedef void(^YYWebImageProgressBlock)(NSInteger receivedSize, NSInteger expectedSize);


/**
 在远程获取图片完成前,调用这个blcok,进行附加的图片处理
 
 @discussion 在 `YYWebImageCompletionBlock`之前调用,给你一个机会对image做附加处理(例如resize或者crop).如果不需要处理,直接返回'image'参数.
 
 @example 例如,你可以使用如下代码进行clip,blur和增加圆角:
 
 ^(UIImage *image, NSURL *url) {
     //也许你需要创建一个@autoreleasepool来限制内存成本。
     image = [image yy_imageByResizeToSize:CGSizeMake(100, 100) contentMode:UIViewContentModeScaleAspectFill];
     image = [image yy_imageByBlurRadius:20 tintColor:nil tintMode:kCGBlendModeNormal saturation:1.2 maskImage:nil];
     image = [image yy_imageByRoundCornerRadius:5];
     return image;
 }
 
 @param image 从url获取的image
 @param url   图片的url (remote or local file path).
 @return 转换后的图片
 */
typedef UIImage * _Nullable (^YYWebImageTransformBlock)(UIImage *image, NSURL *url);


/**
 图像提取完成或取消时调用该block。
 
 @param image       The image.
 @param url         The image url (remote or local file path).
 @param from        Where the image came from.
 @param error       Error during image fetching.
 @param stage       If the operation is cancelled, this value is NO, otherwise YES.
 */
typedef void (^YYWebImageCompletionBlock)(UIImage * _Nullable image, NSURL *url, YYWebImageFromType from,YYWebImageStage stage, NSError * _Nullable error);



/**
 *  创建和管理web image operation的manager
 *
 */
@interface YYWebImageManager : NSObject

+ (instancetype)sharedManager;

/**
 *  使用 image cache 和 operation queue 创建一个manager,
 *  @param cache 被manager使用的image cache(传递nil来避免image cache).
 *  @param queue 操作队列(image operation 在该队列上调度),传递nil使得这个新的operation立即开始without queue
 */
- (instancetype)initWithCache:(nullable YYImageCache *)cache
                        queue:(nullable NSOperationQueue *)queue NS_DESIGNATED_INITIALIZER;
- (instancetype)init UNAVAILABLE_ATTRIBUTE;
+ (instancetype)new UNAVAILABLE_ATTRIBUTE;

/**
 创建并返回一个新的image operation，操作将立即开始。
 @param url        The image url (remote or local file path).
 @param options    The options to control image operation.
 @param progress   Progress block which will be invoked on background thread (pass nil to avoid).
 @param transform  Transform block which will be invoked on background thread  (pass nil to avoid).
 @param completion Completion block which will be invoked on background thread  (pass nil to avoid).
 @return A new image operation.
 */
- (nullable YYWebImageOperation *)requestImageWithURL:(NSURL *)url
                                              options:(YYWebImageOptions)options
                                             progress:(nullable YYWebImageProgressBlock)progress
                                            transform:(nullable YYWebImageTransformBlock)transform
                                           completion:(nullable YYWebImageCompletionBlock)completion;

/**
  供image operation使用的image cache.
  您可以将其设置为nil以避免图像缓存。
  */
@property (nullable, nonatomic, strong) YYImageCache *cache;


/**
  调度和运行 image operations 的操作队列。
  您可以将其设置为nil，使新操作立即开始，不需要队列。
 
  您可以使用此队列来控制最大并发操作数,获取当前operations的状态，或取消该manager的所有operations.
  */
@property (nullable, nonatomic, strong) NSOperationQueue *queue;


/**
  共享的transform block ,用来处理图像。 默认值为nil.
  当调用“requestImageWithURL：options：progress：transform：completion”是,如果`transform`是nil，这个block将被使用。
  */
@property (nullable, nonatomic, copy) YYWebImageTransformBlock sharedTransformBlock;

/**
  图像请求超时间隔（以秒为单位）。 默认值为15。
  */
@property (nonatomic) NSTimeInterval timeout;

/**
  NSURLCredential使用的用户名，默认为nil。
  */
@property (nullable, nonatomic, copy) NSString *username;

/**
  NSURLCredential使用的密码，默认为nil。
  */
@property (nullable, nonatomic, copy) NSString *password;


/**
  图像HTTP请求头。 默认为“Accept：image/webp,image/\*;q = 0.8”.
  */
@property (nullable, nonatomic, copy) NSDictionary<NSString *, NSString *> *headers;


/**
    每次进行image http请求时,调用该block来对http header进行附加处理.默认为nil.
 
    使用此block,针对指定URL,添加或删除HTTP头字段。
  */
@property (nullable, nonatomic, copy) NSDictionary<NSString *, NSString *> *(^headersFilter)(NSURL *url, NSDictionary<NSString *, NSString *> * _Nullable header);

/**
  为每个image operation调用的block。 默认值为nil.
 
  使用此block为指定的URL提供一个自定义的image cache key。
  */
@property (nullable, nonatomic, copy) NSString *(^cacheKeyFilter)(NSURL *url);


/**
  返回指定URL的HTTP头。
 
  @param url 指定的URL.
  @return HTTP headers.
  */
- (nullable NSDictionary<NSString *, NSString *> *)headersForURL:(NSURL *)url;

/**
  返回指定URL的缓存键。
 
  @param url 指定的URL
  @return 在YYImageCache中使用的cache key.
  */
- (NSString *)cacheKeyForURL:(NSURL *)url;


/**
  增加活动网络请求的数量。
  如果这个数字在递增前为零，这将开始状态栏网络活动指示器动画。

  这种方法是线程安全的。
 
  此方法在App Extension中无效。
  */
+ (void)incrementNetworkActivityCount;



/**
  减少活动网络请求的数量。
  如果这个数字在递减后变为零，这将停止状态栏网络活动指示器动画。
 
  这种方法是线程安全的。
 
  此方法在App Extension中无效。
  */
+ (void)decrementNetworkActivityCount;

/**
  获取当前的活动网络请求数。
 
  这种方法是线程安全的。
 
  此方法在App Extension中无效。
  */
+ (NSInteger)currentNetworkActivityCount;

@end


NS_ASSUME_NONNULL_END

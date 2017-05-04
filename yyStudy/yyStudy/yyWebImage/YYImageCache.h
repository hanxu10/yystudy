//
//  YYImageCache.h
//  yyStudy
//
//  Created by hanxu on 2017/5/3.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import <UIKit/UIKit.h>

@class YYMemoryCache, YYDiskCache;

NS_ASSUME_NONNULL_BEGIN

//image缓存类型
typedef NS_OPTIONS(NSUInteger, YYImageCacheType) {
    YYImageCacheTypeNone   = 0,
    YYImageCacheTypeMemory = 1 << 0,///使用内存缓存 获取/存储图像。
    YYImageCacheTypeDisk   = 1 << 1,///使用磁盘缓存获取/存储映像。
    YYImageCacheTypeAll    = YYImageCacheTypeMemory | YYImageCacheTypeDisk,
};

/**
  YYImageCache是一种基于内存缓存和磁盘缓存 来存储UIImage和image data的缓存。
 
  @discussion 磁盘缓存将尝试保护原始图像数据：
 
  *如果原始图像仍然是图像，它将根据alpha信息保存为png / jpeg文件。
  *如果原始图像是gif，apng或webp，那么它将被保存为原始格式。
  *如果原始图像的scale不是1，scale值将被保存为扩展数据。
 
  虽然UIImage可以使用NSCoding协议序列化，但这不是一个好主意：
  苹果实际上使用UIImagePNGRepresentation() 对所有类型的图像进行编码，可能
  丢失原始的多帧数据。 结果是打包到plist文件，不能
  直接使用照片查看器查看。 如果图像没有Alpha通道，则使用JPEG
  而不是PNG可以节省更多的磁盘大小和编码/解码时间。
  */
@interface YYImageCache : NSObject

@property (nullable, copy) NSString *name;//cache的名字,默认为nil
@property (strong, readonly) YYMemoryCache *memoryCache;//底层内存缓存。 有关详细信息，请参阅“YYMemoryCache”.
@property (strong, readonly) YYDiskCache *diskCache;//底层磁盘缓存。 有关详细信息，请参阅“YYDiskCache”.

/**
   是否在从磁盘缓存中提取图像时解码动画图像。 默认为YES。
  
   @discussion 当从磁盘缓存中获取图像时，将使用“YYImage”解码动画图像如WebP/APNG/GIF。设置为“NO”以忽略动画图像。
  */
@property BOOL allowAnimatedImage;


/**
   是否将image解码到内存位图。 默认为YES。
  
   @discussion 如果值为YES，则图像将被解码为内存位图以获得更好的显示性能，但可能花费更多的内存。
  */
@property BOOL decodeForDisplay;


#pragma mark - Initializer
- (instancetype)init UNAVAILABLE_ATTRIBUTE;
+ (instancetype)new UNAVAILABLE_ATTRIBUTE;

+ (instancetype)sharedCache;

/**
   指定初始化器。 具有相同路径的多个实例将使得缓存不稳定。
  
   @param path 目录的完整路径(高速缓存将写入数据到该路径)。一旦初始化，你不应该读写这个目录。
   @result 一个新的缓存对象，如果发生错误，则为nil。
  */
- (nullable instancetype)initWithPath:(NSString *)path NS_DESIGNATED_INITIALIZER;


#pragma mark - Access Methods
/**
   用指定key在缓存(内存和磁盘)中设置image。
   该方法立即返回，并在后台执行存储操作。
  
   @param image 要存储在缓存中的图像。 如果为nil，该方法没有效果。
   @param key 与image关联的key。 如果为nil，该方法没有效果。
  */
- (void)setImage:(UIImage *)image forKey:(NSString *)key;


/**
   在缓存中使用指定的key设置image。
   该方法立即返回，并在后台执行存储操作。
  
   @discussion 如果`type`包含`YYImageCacheTypeMemory`，那么`image`将会存储在内存缓存中; 
               如果`image`为nil，`imageData`将会被使用。
               如果`type`包含`YYImageCacheTypeDisk`，那么`imageData`将会存储在磁盘缓存中;
               如果`imageData`为nil,`image`将会被使用。
  
   @param image 要存储在缓存中的图像。
   @param imageData 要存储在缓存中的图像数据。
   @param key 用于关联图像的键。如果没有，该方法没有效果。
   @param type 要存储图像的缓存类型。
  */
- (void)setImage:(nullable UIImage *)image
       imageData:(nullable NSData *)imageData
          forKey:(NSString *)key
        withType:(YYImageCacheType)type;

/**
   删除缓存中指定键的image(内存和磁盘)。
   该方法立即返回，并在后台执行删除操作。
  
   @param key 标识要删除的图像的键.如果没有，这种方法没有效果。
  */
- (void)removeImageForKey:(NSString *)key;


/**
   删除缓存中指定键的image。
   该方法立即返回，并在后台执行删除操作。
  
   @param key 标识要删除的图像的key.如果为nil，该方法没有效果。
   @param type 要删除图像的缓存类型。
  */
- (void)removeImageForKey:(NSString *)key withType:(YYImageCacheType)type;

/**
   返回一个布尔值，指示给定键是否在缓存中。
   如果图像不在内存中，则此方法可能会阻塞调用线程直到文件读取完成。
  
   @param key 标识图像的字符串。 如果没有，只要返回NO。
   @return 图像是否在缓存中。
  */
- (BOOL)containsImageForKey:(NSString *)key;


/**
   返回一个布尔值，指示给定键是否在缓存中。
   如果image不在内存中，并且`type`包含`YYImageCacheTypeDisk`，此方法可能会阻止调用线程，直到文件读取完成。
  
   @param key 标识图像的字符串。 如果没有，只要返回NO。
   @param type 缓存类型。
   @return 图像是否在缓存中。
  */
- (BOOL)containsImageForKey:(NSString *)key withType:(YYImageCacheType)type;


/**
   返回与给定key相关联的image。
   如果图像不在内存中，则此方法可能会阻塞调用线程直到文件读取完成。
  
   @param key 标识图像的字符串。 如果没有，只返回零点。
   @return 与键相关联的图像，如果没有image与key相关联，则返回nil。
  */
- (nullable UIImage *)getImageForKey:(NSString *)key;


/**
   返回与给定键相关联的图像。
   如果图像不在内存中，并且`type`包含`YYImageCacheTypeDisk`,此方法可能会阻止调用线程，直到文件读取完成。
  
   @param key 标识图像的字符串。 如果为nil，则返回nil。
   @return 与key相关联的image，如果没有image与key相关联，则返回nil。
  */
- (nullable UIImage *)getImageForKey:(NSString *)key withType:(YYImageCacheType)type;


/**
   异步获取与给定键相关联的图像。
  
   @param key 标识图像的字符串。 如果没有，只返回nil。
   @param type 缓存类型。
   @param block 在主线程上调用的完成块。
  */
- (void)getImageForKey:(NSString *)key
              withType:(YYImageCacheType)type
             withBlock:(void(^)(UIImage * _Nullable image, YYImageCacheType type))block;

/**
   返回与给定键相关联的image data。此方法可能阻止调用线程，直到文件读取完成。
  
   @param key 标识图像的字符串。 如果没有，只返回零点。
   @return 与键相关联的图像数据，如果没有图像与该键相关联，则为nil。
  */
- (nullable NSData *)getImageDataForKey:(NSString *)key;


/**
   异步获取与给定键相关联的图像数据。
  
   @param key 标识图像的字符串。 如果没有，只返回零点。
   @param block 在主线程上调用的完成块。
  */
- (void)getImageDataForKey:(NSString *)key
                 withBlock:(void(^)(NSData * _Nullable imageData))block;

@end
NS_ASSUME_NONNULL_END

//
//  YYDiskCache.h
//  yyStudy
//
//  Created by hanxu on 2017/5/4.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import <UIKit/UIKit.h>
NS_ASSUME_NONNULL_BEGIN
/**
 YYDiskCache是一个线程安全的缓存，用于存储由SQLite和file system支持的键值对（类似于NSURLCache的磁盘缓存）。
 YYDiskCache有这些特性:
   *它使用LRU(近期最少使用算法)来删除对象。
   *可以按cost，count和age进行控制。
   *可以配置为在没有可用磁盘空间的情况下自动驱逐对象。
   *它可以自动决定每个对象的存储类型（sqlite / file）以获得更好的性能。
 */
@interface YYDiskCache : NSObject
#pragma mark - Attribute

@property (nullable, copy) NSString *name;
@property (readonly) NSString *path;

/**
    如果对象的数据大小（以字节为单位）大于此值，则对象将被存储为文件，否则对象将存储在sqlite中。
    0表示所有对象将被存储为分离的文件，NSUIntegerMax表示所有对象将存储在sqlite中。
  
    默认值为20480（20KB）。
 */
@property (readonly) NSUInteger inlineThreshold;//内联阈值

/**
    如果此block不为nil，则该block将用来对object进行归档,而不是使用NSKeyedArchiver进行归档。
    您可以使用此block来支持不符合“NSCoding”协议的对象。
    默认值为nil.
 */
@property (nullable, copy) NSData *(^customArchiveBlock)(id object);

/**
 如果此block不为nil，则该block将用来对object进行解档,而不是使用NSUnKeyedArchiver进行解档。
 您可以使用此block来支持不符合“NSCoding”协议的对象。
 默认值为nil.
 */
@property (nullable, copy) id (^customUnarchiveBlock)(NSData *data);

/**
   当一个对象需要保存为一个文件时，这个块将被调用来生成指定键的文件名。 如果该block为nil，使用md5（key）as
   默认文件名。
   默认值为nil。
 */
@property (nullable, copy) NSString *(^customFileNameBlock)(NSString *key);



#pragma mark - Limit
@property NSUInteger countLimit;
@property NSUInteger costLimit;
@property NSTimeInterval ageLimit;

/**
   缓存应保存的最小可用磁盘空间（以字节为单位）。
  
   @discussion 默认值为0，这意味着没有限制。
   如果可用磁盘空间低于此值，缓存将删除对象以释放一些磁盘空间。 这不是一个严格的限制 --- 如果可用磁盘空间
   超过限制，对象可以稍后在后台队列中被逐出。
 */
@property NSUInteger freeDiskSpaceLimit;

/**
   自动修整检查时间间隔（以秒为单位）。 默认为60（1分钟）。
   @discussion 缓存保存一个内部定时器，以检查缓存是否到达它的极限，如果达到极限，它开始驱逐objects。
 */
@property NSTimeInterval autoTrimInterval;

@property BOOL errorLogsEnabled;

#pragma mark - Initializer

- (instancetype)init UNAVAILABLE_ATTRIBUTE;
+ (instancetype)new UNAVAILABLE_ATTRIBUTE;

/**
   根据指定的路径创建一个新的缓存。
  
   @param path 目录的完整路径,往这个路径写数据。 一旦初始化，你不应该读写这个目录。
  
   @return 新的缓存对象，如果发生错误，则为nil。
  
   @warning 如果指定路径的缓存实例已经存在于内存中，则此方法将直接返回，而不是创建新实例。
 */
- (nullable instancetype)initWithPath:(NSString *)path;

/**
   指定的初始化器。
  
   @param path 目录的完整路径,往这个路径写数据。 一旦初始化，你不应该读写这个目录。
  
   @param threshold 数据存储内联阈值（以字节为单位）。 如果对象的数据大小（以字节为单位）大于此值，则对象将被存储为文件，否则对象将存储在sqlite中。 0表示所有对象将被存储为分离的文件，NSUIntegerMax表示所有对象将存储在sqlite中。 如果你不知道你的对象的大小，20480是一个不错的选择。 首次初始化后，不应更改指定路径的值。
  
   @return 新的缓存对象，如果发生错误，则为nil。
  
   @warning 如果指定路径的缓存实例已经存在于内存中，则此方法将直接返回，而不是创建新实例。
 */
- (nullable instancetype)initWithPath:(NSString *)path
                      inlineThreshold:(NSUInteger)threshold NS_DESIGNATED_INITIALIZER;


#pragma mark - Access Methods
/**
   返回一个布尔值，指示给定键是否在缓存中。
   此方法可能阻止调用线程，直到文件读取完成。
  
   @param key 标识值的字符串。 如果为nil，只要返回NO。
   @return key是否在缓存中。
 */
- (BOOL)containsObjectForKey:(NSString *)key;

- (void)containsObjectForKey:(NSString *)key withBlock:(void(^)(NSString *key, BOOL contains))block;

/**
   返回与给定键相关联的值。
   此方法可能阻止调用线程，直到文件读取完成。
  
   @param key 标识值的字符串。 如果没有，只返回零点。
   @return 与键相关联的值，如果没有值与键相关联，则为nil。
 */
- (nullable id<NSCoding>)objectForKey:(NSString *)key;
- (void)objectForKey:(NSString *)key withBlock:(void(^)(NSString *key, id<NSCoding> _Nullable object))block;


- (void)setObject:(nullable id<NSCoding>)object forKey:(NSString *)key;
- (void)setObject:(nullable id<NSCoding>)object forKey:(NSString *)key withBlock:(void(^)(void))block;


- (void)removeObjectForKey:(NSString *)key;
- (void)removeObjectForKey:(NSString *)key withBlock:(void(^)(NSString *key))block;

- (void)removeAllObjects;
- (void)removeAllObjectsWithBlock:(void(^)(void))block;


- (void)removeAllObjectsWithProgressBlock:(nullable void(^)(int removedCount, int totalCount))progress
                                 endBlock:(nullable void(^)(BOOL error))end;



- (NSInteger)totalCount;
- (void)totalCountWithBlock:(void(^)(NSInteger totalCount))block;


- (NSInteger)totalCost;
- (void)totalCostWithBlock:(void(^)(NSInteger totalCost))block;


#pragma mark - Trim
/**
   用LRU从缓存中删除对象，直到`totalCount`低于或等于指定值.可能阻塞调用线程.
   @param count 缓存被修剪后允许保留的总计数。
 */
- (void)trimToCount:(NSUInteger)count;

/**
 用LRU从缓存中删除对象，直到`totalCount`低于或等于指定值.
 当操作完成后再后台执行block.
 */
- (void)trimToCount:(NSUInteger)count withBlock:(void(^)(void))block;


- (void)trimToCost:(NSUInteger)cost;
- (void)trimToCost:(NSUInteger)cost withBlock:(void(^)(void))block;


- (void)trimToAge:(NSTimeInterval)age;
- (void)trimToAge:(NSTimeInterval)age withBlock:(void(^)(void))block;


#pragma mark - Extended Data
/**
   从对象获取扩展数据。
  
   @discussion 有关详细信息，请参阅'setExtendedData：toObject：'。
   @param object 一个对象。
   @return 扩展数据。
 */
+ (nullable NSData *)getExtendedDataFromObject:(id)object;

/**
   将扩展数据设置到一个对像。
  
   @discussion 您可以在 把对象保存到磁盘缓存之前，将任何扩展数据设置给对象。 扩展数据也将与此对象一起保存。 稍后可以使用“getExtendedDataFromObject：”获取扩展数据。
  
   @param extendedData 扩展数据(pass nil to remove)。
   @param object 对象。
 */
+ (void)setExtendedData:(nullable NSData *)extendedData toObject:(id)object;
@end
NS_ASSUME_NONNULL_END

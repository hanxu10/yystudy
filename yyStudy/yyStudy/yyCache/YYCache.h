//
//  YYCache.h
//  yyStudy
//
//  Created by hanxu on 2017/5/4.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN


/**
   `YYCache`是线程安全的键值缓存。
  
   它使用“YYMemoryCache”将对象存储在一个小而快速的内存缓存中，
   并使用“YYDiskCache”将对象持久化到一个大而慢的磁盘缓存。
   有关详细信息，请参阅“YYMemoryCache”和“YYDiskCache”。
 */
@interface YYCache : NSObject
@property (copy, readonly) NSString *name;//cache的名字
@property (strong, readonly) YYMemoryCache *memoryCache;//内存缓存
@property (strong, readonly) YYDiskCache *diskCache;//磁盘缓存


/**
   用指定名称创建一个新实例。
   具有相同名称的多个实例将使缓存不稳定。
  
   @param name 缓存的名称。 在app的cache dictionary中使用name创建一个字典,用于磁盘缓存.
               一旦初始化，你不应该读写此目录。
   @result 一个新的缓存对象，如果发生错误，则为nil。
  */
- (nullable instancetype)initWithName:(NSString *)name;


/**
   使用指定的路径创建一个新的实例。
   具有相同名称的多个实例将使缓存不稳定。
  
   @param path 高速缓存将写入数据 到 这个目录路径。
               一旦初始化，你不应该读写这个目录。
   @result 一个新的缓存对象，如果发生错误，则为nil。
  */
- (nullable instancetype)initWithPath:(NSString *)path NS_DESIGNATED_INITIALIZER;


/**
   便利初始化器
   创建一个具有指定名称的新实例。
   具有相同名称的多个实例将使缓存不稳定。
  
   @param name  cache的名称。 在app的cache dictionary中使用name创建一个字典,用于磁盘缓存。
                一旦初始化，你不应读写此目录。
   @result 一个新的缓存对象，如果发生错误，则为nil。
  */
+ (nullable instancetype)cacheWithName:(NSString *)name;

+ (nullable instancetype)cacheWithPath:(NSString *)path;

- (instancetype)init UNAVAILABLE_ATTRIBUTE;
+ (instancetype)new UNAVAILABLE_ATTRIBUTE;


#pragma mark - Access Methods
/**
   返回一个布尔值，指示给定键是否在缓存中。
   此方法可能阻止调用线程，直到文件读取完成。
  
   @param key 标识值的字符串。 如果没有，返回NO。
   @return key是否在缓存中。
 */
- (BOOL)containsObjectForKey:(NSString *)key;

/**
   在block中返回一个布尔值，指示给定键是否在缓存中。
   此方法立即返回并调用后台队列中传递的块操作完成。
  
   @param key 标识值的字符串。 如果没有，只要返回NO。
   @param block 完成后将在后台队列中调用的块。
 */
- (void)containsObjectForKey:(NSString *)key withBlock:(nullable void(^)(NSString *key, BOOL contains))block;

/**
   返回与给定键相关联的值。
   此方法可能阻止调用线程，直到文件读取完成。
  
   @param key 标识值的字符串。 如果没有，只返回零点。
   @return 与键相关联的值，如果没有值与键相关联，则为nil。
 */
- (nullable id<NSCoding>)objectForKey:(NSString *)key;


/**
   返回与给定键相关联的值。
   此方法立即返回.当操作完成时,在后台队列执行该block
   
   @param key 标识值的字符串。 如果nil，返回nil。
   @param block 完成后将在后台队列中调用的块。
 */
- (void)objectForKey:(NSString *)key withBlock:(nullable void(^)(NSString *key, id<NSCoding> object))block;


/**
   此方法可能会阻止调用线程，直到文件写入完成。
  
   @param object 要存储在缓存中的对象。 如果没有，它调用`removeObjectForKey：`。
   @param key 用于关联该值的键。 如果为nil，这种方法没有效果。
  */
- (void)setObject:(nullable id<NSCoding>)object forKey:(NSString *)key;


/**
   此方法立即返回并调用后台队列中传递的块操作完成。
  
   @param object 要存储在缓存中的对象。 如果没有，它调用`removeObjectForKey：`。
   @param block 完成后将在后台队列中调用的块。
  */
- (void)setObject:(nullable id<NSCoding>)object forKey:(NSString *)key withBlock:(nullable void(^)(void))block;


/**
   删除缓存中指定键的值。
   此方法可能会阻止调用线程，直到文件删除完成。
  
   @param key 标识要删除的值的键。 如果为nil，该方法没有效果。
  */
- (void)removeObjectForKey:(NSString *)key;


/**
   删除缓存中指定键的值。
   此方法立即返回.当操作完成时在后台队列执行传递的块。
  
   @param key 标识要删除的值的键。 如果为nil，这种方法没有效果。
   @param block 完成后将在后台队列中调用的块。
  */
- (void)removeObjectForKey:(NSString *)key withBlock:(nullable void(^)(NSString *key))block;


/**
   清空缓存。
   此方法可能会阻止调用线程，直到文件删除完成。
 */
- (void)removeAllObjects;

/**
   清空缓存。
   此方法立即返回.当操作完成时,在后台队列执行block.
  
   @param block 完成后将在后台队列中调用的块。
  */
- (void)removeAllObjectsWithBlock:(void(^)(void))block;


/**
   清空缓存with block。
   该方法立即返回，并在后台使用block执行清除操作。
  
   @warning 您不应该在这些块中向此实例发送消息。
   @param progress 这个block将在删除期间被调用，传入nil来忽略。
   @param end 这个块将在末尾被调用，传入nil来忽略。
  */
- (void)removeAllObjectsWithProgressBlock:(nullable void(^)(int removedCount, int totalCount))progress
                                 endBlock:(nullable void(^)(BOOL error))end;
@end

NS_ASSUME_NONNULL_END

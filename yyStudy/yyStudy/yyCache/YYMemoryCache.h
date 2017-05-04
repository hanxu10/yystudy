//
//  YYMemoryCache.h
//  yyStudy
//
//  Created by hanxu on 2017/5/4.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import <UIKit/UIKit.h>
NS_ASSUME_NONNULL_BEGIN

/**
   YYMemoryCache是一个快速的内存缓存，用于存储键值对。
   与NSDictionary相反，key被保留并且不被copied。
   
   API和性能类似于“NSCache”，所有方法都是线程安全的。
  
   YYMemoryCache对象与NSCache的不同之处在于：
   *它使用LRU（近期最少使用算法）来删除对象; NSCache的驱逐方式是非确定性的。
   *可以按cost，count和age进行控制; NSCache的限制是不精确的。
   *它可以配置为在接收内存警告或应用程序进入后台时自动驱逐对象。
  
   YYMemoryCache中的“访问方法”的时间通常是恒定的(O(1))。
  */
@interface YYMemoryCache : NSObject
@property (nullable, copy) NSString *name;//缓存的名称。 默认值为nil
@property (readonly) NSUInteger totalCount;//缓存中的对象数(只读)
@property (readonly) NSUInteger totalCost;//缓存中对象的总容量(只读)


#pragma mark - Limit
/**
   缓存应该保存的最大对象数。
  
   @discussion 默认值为NSUIntegerMax，这意味着没有限制。
   这不是严格的限制 --- 如果缓存超过限制，缓存中的某些对象可能会在后台线程中被逐出。
  */
@property NSUInteger countLimit;


/**
   缓存开始驱逐对象之前可以容纳的最大总容量。
  
   @discussion 默认值为NSUIntegerMax，这意味着没有限制。
   这不是严格的限制 --- 如果缓存超过限制，缓存中的某些对象可能会在后台线程中被逐出。
  */
@property NSUInteger costLimit;


/**
   缓存中对象的最大到期时间。
  
   @discussion 默认值为DBL_MAX，这意味着没有限制。 
        这不是一个严格的限制 - 如果一个对象超过极限，则该对象可以在后面的线程中被逐出。
  */
@property NSTimeInterval ageLimit;


/**
   自动修整检查时间间隔（以秒为单位）。 默认值为5.0。
  
   @discussion缓存保存一个内部定时器，以检查缓存是否达到限制，如果达到限制，它将开始驱逐对象。
  */
@property NSTimeInterval autoTrimInterval;


/**
  如果“YES”，缓存将在应用程序收到内存警告时删除所有对象。
  默认值为“YES”。
 */
@property BOOL shouldRemoveAllObjectsOnMemoryWarning;


/**
   如果“YES”，则当应用程序进入后台时，缓存将会删除所有对象。
   默认值为“YES”。
 */
@property BOOL shouldRemoveAllObjectsWhenEnteringBackground;

/**
   应用程序收到内存警告时执行的block.
   默认值为nil。
 */
@property (nullable, copy) void(^didReceiveMemoryWarningBlock)(YYMemoryCache *cache);

/**
   当应用程序进入后台时要执行的block.
   默认值为nil.
 */
@property (nullable, copy) void(^didEnterBackgroundBlock)(YYMemoryCache *cache);

/**
   如果“YES”，键值对将在主线程上释放，否则在后台线程上。 默认为NO。
   @discussion 如果键值对象包含应该在主线程中释放的实例(如UIView/CALayer)，则可以将此值设置为“YES”。
 */
@property BOOL releaseOnMainThread;


/**
   如果“YES”，键值对将被异步释放，以避免阻塞访问方法，否则将在访问方法(例如removeObjectForKey:)中释放。
   默认为YES。
  */
@property BOOL releaseAsynchronously;


#pragma mark - Access Methods
/**
  返回一个布尔值，指示给定键是否在缓存中。
  @param key 标识值的对象。 如果为nil，只需返回`NO`。
  @return key是否在缓存中。
 */
- (BOOL)containsObjectForKey:(id)key;

/**
   返回与给定键相关联的值。
   @param key 标识值的对象。 如果为nil，则返回nil。
   @return 与键相关联的值，如果没有值与键相关联，则为nil。
 */
- (nullable id)objectForKey:(id)key;


/**
   设置缓存中指定键的值（0cost）。
  
   @param object 要存储在缓存中的对象。 如果nil，它调用removeObjectForKey：`。
   @param key 用于关联该值的键。 如果为nil，该方法无效果。
   @discussion 与NSMutableDictionary对象不同，cache不会把key对象copy进来。
 */
- (void)setObject:(nullable id)object forKey:(id)key;

/**
   设置缓存中指定键的值，并将键值对与指定的cost相关联。
  
   @param object 要存储在缓存中的对象。 如果为nil，它调用`removeObjectForKey`。
   @param key 用于关联该值的键。 如果为nil，这种方法没有效果。
   @param cost 与键值对相关联的成本。
   @discussion 与NSMutableDictionary对象不同，高速缓存不会复制放在其中的关键对象。
 */
- (void)setObject:(nullable id)object forKey:(id)key withCost:(NSUInteger)cost;

/**
   删除缓存中指定键的值。
  
   @param key 标识要删除的值的键。 如果为nil，这种方法没有效果。
  */
- (void)removeObjectForKey:(id)key;

/**
   立即清空缓存。
 */
- (void)removeAllObjects;


#pragma mark - Trim
/**
   用LRU从缓存中删除对象，直到`totalCount`低于或等于指定值。
   @param count 缓存被修剪后允许保留的总计数。
 */
- (void)trimToCount:(NSUInteger)count;

/**
   用LRU从缓存中删除对象，直到“totalCost”等于指定值。
   @param cost 缓存被修剪后允许的总成本。
 */
- (void)trimToCost:(NSUInteger)cost;

/**
   用LRU从缓存中删除对象，直到所有到期对象根据指定的值被删除。
   @param age 对象的最大年龄（以秒为单位）。
 */
- (void)trimToAge:(NSTimeInterval)age;
@end

NS_ASSUME_NONNULL_END

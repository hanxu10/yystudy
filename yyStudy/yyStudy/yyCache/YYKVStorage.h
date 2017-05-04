//
//  YYKVStorage.h
//  yyStudy
//
//  Created by hanxu on 2017/5/4.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import <Foundation/Foundation.h>
NS_ASSUME_NONNULL_BEGIN


/**
   “YYKVStorage”使用YYKVStorageItem来存储键值对和元数据。
   通常，你不应该直接使用这个类。
  */
@interface YYKVStorageItem : NSObject
@property (nonatomic, strong) NSString *key;
@property (nonatomic, strong) NSString *value;
@property (nullable, nonatomic, strong) NSString *filename;
@property (nonatomic) int size;//value的size(in bytes)
@property (nonatomic) int modTime;//修改时间(unix timestamp)
@property (nonatomic) int accessTime;//最后访问时间(unix timestamp)
@property (nullable, nonatomic, strong) NSData *extendedData;//额外数据(nil if no extended data)
@end


/**
   存储类型指示“YYKVStorageItem.value”存储在哪里.
   @discussion 通常，将数据写入sqlite比写入到extern file更快，但读取性能取决于数据大小。 在我的测试中（在iPhone 6 64G上），当数据大于20KB时，从extern file 读取数据比sqlite更快。
  
   *如果要存储大量小数据（如联系人缓存），请使用YYKVStorageTypeSQLite来获得更好的性能。
   *如果要存储大文件（如图像缓存），请使用YYKVStorageTypeFile获得更好的性能。
   *您可以使用YYKVStorageTypeMixed，并为每个item选择您的存储类型。
  
   有关详细信息，请参阅<http://www.sqlite.org/intern-v-extern-blob.html>。
  */
typedef NS_ENUM(NSUInteger, YYKVStorageType) {
    YYKVStorageTypeFile = 0,
    YYKVStorageTypeSQLite = 1,
    YYKVStorageTypeMixed = 2,//“value”根据您的选择存储在文件系统或sqlite中。
};



/**
   YYKVStorage是基于sqlite和文件系统的键值存储。 通常，你不应该直接使用这个类。
  
   @discussion YYKVStorage的指定的初始化器是`initWithPath：type：`。 初始化后，将基于“path”创建一个目录,用来保存键值数据。 一旦初始化，您不应该在没有YYKVStorage实例的情况下读取或写入该目录。
  
   您可以编译最新版本的sqlite，并忽略iOS系统中的libsqlite3.dylib，以获得2x〜4x的速度。
  
   @warning 这个类的实例是*NOT*线程安全，你需要确保只有一个线程同时访问该实例。 如果你真的
   需要在多线程中处理大量的数据，应该将数据分割成多个KVStorage实例（分片）。
  */
@interface YYKVStorage : NSObject

#pragma mark - Attribute
@property (nonatomic, readonly) NSString *path;        //存储的路径
@property (nonatomic, readonly) YYKVStorageType type;  //存储的type
@property (nonatomic) BOOL errorLogsEnabled;

#pragma mark - Initializer
- (instancetype)init UNAVAILABLE_ATTRIBUTE;
+ (instancetype)new UNAVAILABLE_ATTRIBUTE;

/**
   指定的初始化器。
  
   @param path 将写入数据的目录的完整路径。 如果目录不存在，它将尝试创建一个目录，否则将读取此目录中的数据。
   @param type 存储类型。 首次初始化后，不应更改指定路径的type。
   @return 新的存储对象，如果发生错误，则为nil。
   @warning 具有相同路径的多个实例将使存储不稳定。
  */
- (nullable instancetype)initWithPath:(NSString *)path type:(YYKVStorageType)type NS_DESIGNATED_INITIALIZER;


#pragma mark - Save Items
/**
   保存item 或使用“key”更新item，如果它已经存在。
  
   @discussion 此方法将item.key，item.value，item.filename和item.extendedData保存到磁盘或sqlite，其他属性将被忽略。 item.key和item.value不应为空(nil或零长度).
  
   如果`type`是YYKVStorageTypeFile，那么item.filename不应该是空的。
   如果`type`是YYKVStorageTypeSQLite，那么item.filename将被忽略。
   如果`type`是YYKVStorageTypeMixed，那么如果item.filename不为空，则item.value将被保存到文件系统，否则将被保存到sqlite。
  
   @param item  一个item.
   @return  是否成功
  */
- (BOOL)saveItem:(YYKVStorageItem *)item;


/**
   保存item或使用“key”更新item，如果它已经存在。
  
   @discussion 此方法将键值对保存到sqlite.
               如果`type`是YYKVStorageTypeFile，那么这个方法就会失败。
  
   @param key 不应为空（nil或零长度）。
   @param value 不应为空（nil或零长度）。
   @return 是否成功
  */
- (BOOL)saveItemWithKey:(NSString *)key value:(NSData *)value;

/**
   保存项目或使用“key”更新项目，如果它已经存在。
  
   @discussion
   如果`type`是YYKVStorageTypeFile，那么`filename`不应该是空的。
   如果`type`是YYKVStorageTypeSQLite，那么`filename`将被忽略。
   如果`type`是YYKVStorageTypeMixed，(如果是filename不为空,则存到文件系统,否则存到sqlite).
  
   @param key 不应为空（零或零长度）。
   @param value 不应为空（零或零长度）。
   @param filename 文件名。
   @param extendedData 该项目的扩展数据（pass nil to ignore it）。
  
   @return 是否成功
  */
- (BOOL)saveItemWithKey:(NSString *)key
                  value:(NSData *)value
               filename:(nullable NSString *)filename
           extendedData:(nullable NSData *)extendedData;

#pragma mark - Remove Items




@end
NS_ASSUME_NONNULL_END

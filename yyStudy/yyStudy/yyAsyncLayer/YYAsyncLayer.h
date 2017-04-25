//
//  YYAsyncLayer.h
//  yyStudy
//
//  Created by hanxu on 2017/4/24.
//  Copyright © 2017年 hanxu. All rights reserved.
//
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
NS_ASSUME_NONNULL_BEGIN
////////////////////////////////////////////////////////////////////////////////////
/**
 *  YYAsyncLayer是CALayer的子类,用于异步渲染内容.
 *
 *  @discussion 当layer需要更新它的内容时,它将请求代理,获取一个异步的task来在后台渲染内容.
 */
@interface YYAsyncLayer : CALayer
//render的代码是否在后台执行.默认是yes.
@property (nonatomic, assign) BOOL displaysAsynchronously;
@end
////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////
@class YYAsyncLayerDisplayTask;

@protocol YYAsyncLayerDelegate <NSObject>

@required
//当layer的内容需要更新时,调用这个方法获得一个新的display task
- (YYAsyncLayerDisplayTask *)newAsyncDisplayTask;

@end
////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////
/**
 *  YYAsyncLayer用来在后台渲染的display task
 */
@interface YYAsyncLayerDisplayTask : NSObject

//异步绘制开始前调用,在主线程
@property (nonatomic, copy, nullable) void (^willDisplay)(CALayer *layer);

/**
 *  这个block被调用用来绘制layer的内容
 *  @discussion 这个block可能在主线程和后台线程调用,所以它应该是线程安全的.
 *  @param context          一个由layer创建的新的bitmap content
 *  @param size             content的size(一般情况下,等于layer的bound size)
 *  @param isCancelled      如果这个block返回yes.这个方法会取消绘制过程并尽快return.
 */
@property (nonatomic, copy, nullable) void (^display)(CGContextRef context, CGSize size, BOOL(^isCancelled)(void));

/**
 *  这个block将在异步绘制完成后调用.在主线程.
 *
 *  如果draw过程被取消了,finished为NO,否则为YES.
 */
@property (nonatomic, copy, nullable) void (^didDisplay)(CALayer *layer, BOOL finished);

@end
////////////////////////////////////////////////////////////////////////////////////

NS_ASSUME_NONNULL_END

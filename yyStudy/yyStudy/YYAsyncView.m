//
//  YYAsyncView.m
//  yyStudy
//
//  Created by hanxu on 2017/4/25.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import "YYAsyncView.h"
#import "yyAsyncLayer/YYAsyncLayer.h"
#import "yyAsyncLayer/YYTransaction.h"

@implementation YYAsyncView

+ (Class)layerClass
{
    return [YYAsyncLayer class];
}

- (void)setText:(NSString *)text
{
    _text = text;
    [[YYTransaction transactionWithTarget:self selector:@selector(contentsNeedUpdate)] commit];
}

- (void)setFont:(UIFont *)font
{
    _font = font;
    [[YYTransaction transactionWithTarget:self selector:@selector(contentsNeedUpdate)] commit];
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    [[YYTransaction transactionWithTarget:self selector:@selector(contentsNeedUpdate)] commit];
}

- (void)contentsNeedUpdate
{
    [self.layer setNeedsDisplay];
}

- (YYAsyncLayerDisplayTask *)newAsyncDisplayTask
{
    NSString *text = _text;
    UIFont *font = _font;
    
    YYAsyncLayerDisplayTask *task = [YYAsyncLayerDisplayTask new];
    task.willDisplay = ^(CALayer * _Nonnull layer) {
        
    };
    task.display = ^(CGContextRef  _Nonnull context, CGSize size, BOOL (^ _Nonnull isCancelled)(void)) {
        if (isCancelled()) {
            return ;
        }
        
        CGRect rect = self.bounds;
        
        [text drawInRect:rect withAttributes:@{NSFontAttributeName : font}];
    };
    task.didDisplay = ^(CALayer * _Nonnull layer, BOOL finished) {
        
    };
    return task;
}


@end

//
//  YYTransaction.m
//  yyStudy
//
//  Created by hanxu on 2017/4/24.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import "YYTransaction.h"
@interface YYTransaction ()
@property (nonatomic, strong) id target;
@property (nonatomic, assign) SEL selector;
@end



static NSMutableSet *transactionSet = nil;

static void YYRunLoopObserverCallBack(CFRunLoopObserverRef observer, CFRunLoopActivity activity, void *info)
{
    if (transactionSet.count == 0) {
        return;
    }
    NSSet *currentSet = transactionSet;
    transactionSet = [NSMutableSet set];
    [currentSet enumerateObjectsUsingBlock:^(YYTransaction *transaction, BOOL *stop) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Warc-performSelector-leaks"
        [transaction.target performSelector:transaction.selector];
#pragma clang diagnostic pop
    }];
}

static void YYTransactionSetup()
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        transactionSet = [NSMutableSet set];
        CFRunLoopRef runloop = CFRunLoopGetMain();
        CFRunLoopObserverRef observer;
        
        observer = CFRunLoopObserverCreate(CFAllocatorGetDefault(), kCFRunLoopBeforeWaiting | kCFRunLoopExit, YES
                                           , 0xFFFFFF, YYRunLoopObserverCallBack, NULL);
        CFRunLoopAddObserver(runloop, observer, kCFRunLoopCommonModes);
        CFRelease(observer);
    });
}





@implementation YYTransaction
+ (YYTransaction *)transactionWithTarget:(id)target selector:(SEL)selector
{
    if (!target || !selector) {
        return nil;
    }
    YYTransaction *transaction = [YYTransaction new];
    transaction.target = target;
    transaction.selector = selector;
    return transaction;
}

- (void)commit
{
    if (!_target || !_selector) {
        return;
    }
    YYTransactionSetup();
    [transactionSet addObject:self];
}


- (NSUInteger)hash {
    long v1 = (long)((void *)_selector);
    long v2 = (long)_target;
    return v1 ^ v2;
}

- (BOOL)isEqual:(id)object {
    if (self == object) return YES;
    if (![object isMemberOfClass:self.class]) return NO;
    YYTransaction *other = object;
    return other.selector == _selector && other.target == _target;
}
@end

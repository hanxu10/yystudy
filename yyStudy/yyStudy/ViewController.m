//
//  ViewController.m
//  yyStudy
//
//  Created by hanxu on 2017/4/24.
//  Copyright © 2017年 hanxu. All rights reserved.
//

#import "ViewController.h"
#import "YYAsyncView.h"
@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    YYAsyncView *yyV = [[YYAsyncView alloc] initWithFrame:CGRectMake(100, 100, 100, 100)];
    yyV.backgroundColor = [UIColor redColor];
    [yyV setText:@"哈哈哈"];
    [yyV setFont:[UIFont systemFontOfSize:18]];
    [self.view addSubview:yyV];
    
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end

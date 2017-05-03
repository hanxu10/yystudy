//
//  Introduce.h
//  yyStudy
//
//  Created by hanxu on 2017/4/27.
//  Copyright © 2017年 hanxu. All rights reserved.
//


//主线程一旦出现繁重的任务,就会导致界面卡顿,主要有三类这样的任务:
//1.排版:
//    视图大小计算
//    文本高度计算
//    重新计算子视图的排版
//
//2.绘制:
//    文本绘制(如CoreText)
//    图片绘制(如预先解压)
//    元素绘制(如Quartz)
//
//3.UI对象操作:
//    UIView/CALayer等UI对象的创建

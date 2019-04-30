//
//  CppObjCMM.m
//  CPlusplus
//
//  Created by msftaip 1 on 4/29/19.
//  Copyright © 2019 msftaip 1. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CppObjCMM.h"
#include "Calculator.hpp"

@interface CppObjCMM()
@end

@implementation CppObjCMM

-(CppObjCMM *)init:(NSInteger)a andOtherInt:(NSInteger) b{
    if(!_cal)
    {
        _cal = new Elementary::Math::Calculator(a, b);
    }
    CppObjCMM * cppObjCmm =[[CppObjCMM alloc] init];
    return cppObjCmm;
}

-(NSInteger) Add: (NSInteger)a andOtherInt:(NSInteger) b
{
    //Elementary::Math::Calculator *cal = new Elementary::Math::Calculator();
    return _cal->Add(a, b);
}

-(NSInteger) Minus: (NSInteger)a andOtherInt: (NSInteger) b
{
    //Elementary::Math::Calculator *cal = new Elementary::Math::Calculator();
    return _cal->Minus(a, b);
}

@end


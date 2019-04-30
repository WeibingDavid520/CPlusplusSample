//
//  CppObjCMM.h
//  CPlusplus
//
//  Created by msftaip 1 on 4/29/19.
//  Copyright © 2019 msftaip 1. All rights reserved.
//

#ifndef CppObjCMM_h
#define CppObjCMM_h
#import <Foundation/Foundation.h>
#include "Calculator.hpp"

@interface CppObjCMM: NSObject
    -(CppObjCMM*)init:(NSInteger)a andOtherInt:(NSInteger) b;
    -(NSInteger) Add: (NSInteger)a andOtherInt:(NSInteger) b;
    -(NSInteger) Minus: (NSInteger)a andOtherInt: (NSInteger) b;
   @property Elementary::Math::Calculator *cal;
@end

#endif /* CppObjCMM_h */

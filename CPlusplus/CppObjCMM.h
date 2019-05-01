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
#include "./upe/samples/upe/action.h"

@interface CppObjCMM: NSObject
    -(CppObjCMM*)init:(NSInteger)a andOtherInt:(NSInteger) b;
    -(NSInteger) Add: (NSInteger)a andOtherInt:(NSInteger) b;
    -(NSInteger) Minus: (NSInteger)a andOtherInt: (NSInteger) b;
    @property Elementary::Math::Calculator *cal;
    @property sample::upe::Action *action;
    @property sample::upe::AuthenticationType * authenticationType;
    @property sample::upe::AuthenticationOptions * authenticationOptions;
    @property sample::upe::PolicyType * policyType;
    @property sample::upe::ProfileOptions * profileOptions;

@end

#endif /* CppObjCMM_h */

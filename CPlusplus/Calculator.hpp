//
//  Calculator.hpp
//  CPlusplus
//
//  Created by msftaip 1 on 4/29/19.
//  Copyright © 2019 msftaip 1. All rights reserved.
//

#ifndef Calculator_hpp
#define Calculator_hpp

#include <stdio.h>
namespace Elementary
{
    namespace Math
    {
        class Calculator
        {
        public:
            Calculator(){};
            int Add(int a, int b);
            int Minus(int a, int b);
        };
    }
}
#endif /* Calculator_hpp */

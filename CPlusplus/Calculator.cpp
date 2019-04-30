//
//  Calculator.cpp
//  CPlusplus
//
//  Created by msftaip 1 on 4/29/19.
//  Copyright © 2019 msftaip 1. All rights reserved.
//

#include "Calculator.hpp"
namespace Elementary
{
    namespace Math
    {
        Calculator::Calculator(int a, int b)
        {
            aa = a;
            bb = b;
        }
        
        int Calculator::Add(int a, int b)
        {
            return a + b;
        }
        
        int Calculator::Minus(int a, int b)
        {
            return a - b;
        }
    }
}

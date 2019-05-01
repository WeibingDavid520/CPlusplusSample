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
#include<iostream>

namespace Elementary
{
    namespace Math
    {
        class Calculator
        {
        public:
            Calculator(const int& a);
            Calculator(int a, int b);
            int Add(int a, int b);
            int Minus(int a, int b);
        private:
            int aa;
            int bb;
        };
    }
}
#endif /* Calculator_hpp */

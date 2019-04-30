//
//  ViewController.m
//  CPlusplus
//
//  Created by msftaip 1 on 4/29/19.
//  Copyright © 2019 msftaip 1. All rights reserved.
//

#import "ViewController.h"
#import "CppObjCMM.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    

    
    
}

- (IBAction)Add:(id)sender {
        CppObjCMM * cppObj = [[CppObjCMM alloc] init];
    NSInteger a = [_atf.text intValue];
    NSInteger b = [_btf.text intValue];
    _addResult.text = [@([cppObj Add: a andOtherInt:b]) stringValue];
}

- (IBAction)Minus:(id)sender {
    CppObjCMM * cppObj = [[CppObjCMM alloc] init];
    NSInteger a = [_atf.text intValue];
    NSInteger b = [_btf.text intValue];
    _minusResult.text = [@([cppObj Minus: a andOtherInt:b]) stringValue];
}


@end

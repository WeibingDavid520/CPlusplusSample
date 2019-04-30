//
//  ViewController.h
//  CPlusplus
//
//  Created by msftaip 1 on 4/29/19.
//  Copyright © 2019 msftaip 1. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController

@property (weak, nonatomic) IBOutlet UITextField *atf;

@property (weak, nonatomic) IBOutlet UITextField *btf;

//@property (weak, nonatomic) IBOutlet UIButton *minus;
//@property (weak, nonatomic) IBOutlet UIButton *add;

@property(strong, nonatomic)IBOutlet UILabel *minusResult;

@property(strong, nonatomic)IBOutlet UILabel *addResult;


//    -(IBAction)Add:(id)sender;
@end


//
//  W2NibCell.h
//
//  Created by Vitaliy Ivanov on 12/13/11.
//  Copyright (c) 2011 w2f2. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface W2NibCell : UITableViewCell

+ (NSString*)reuseIdentifier;
+ (W2NibCell*)dequeueFromTableView:(UITableView*)tableView;

@property (readwrite, strong, nonatomic) IBOutlet UIView* view;

@end

//
//  W2NibCell.m
//
//  Created by Vitaliy Ivanov on 12/13/11.
//  Copyright (c) 2011 w2f2. All rights reserved.
//

#import "W2NibCell.h"

@implementation W2NibCell

@synthesize view = view_;

- (id)init
{
	if ((self = [super initWithStyle:UITableViewCellStyleDefault
		reuseIdentifier:[[self class] reuseIdentifier]]))
	{
		[[NSBundle mainBundle] loadNibNamed:NSStringFromClass([self class])
			owner:self options:nil];
		
		self.backgroundColor = self.contentView.backgroundColor = [UIColor clearColor];
	}
	
    return self;
}

- (void)setView:(UIView *)view
{
	view_ = view;
	[self.contentView addSubview:view_];
	view_.frame = CGRectMake(0, 0, [self.contentView bounds].size.width, [self.contentView bounds].size.height);
}

+ (W2NibCell*)dequeueFromTableView:(UITableView*)tableView
{
	W2NibCell* cell = [tableView dequeueReusableCellWithIdentifier:[self reuseIdentifier]];
	if (!cell)
		cell = [[self alloc] init];
	return cell;
}

+ (NSString*)reuseIdentifier
{
	NSString* s = NSStringFromClass(self);
	return s;
}

@end

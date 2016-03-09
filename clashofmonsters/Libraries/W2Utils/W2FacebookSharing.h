//
//  W2FacebookSharing.h
//
//  Created by Vitaliy Ivanov on 5/7/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface W2FacebookSharing : NSObject
{
	
}

- (void)shareWithCompletion:(void(^)(BOOL published, NSError* error))completion;
- (void)inviteWithCompletion:(void(^)(BOOL published, NSError* error))completion;

@property (readwrite, retain, nonatomic) NSURL* link;
@property (readwrite, retain, nonatomic) NSURL* picture;
@property (readwrite, retain, nonatomic) NSString* name;
@property (readwrite, retain, nonatomic) NSString* caption;
@property (readwrite, retain, nonatomic) NSString* description;

@end

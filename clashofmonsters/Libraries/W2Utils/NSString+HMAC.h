//
//  NSString+HMAC.h
//
//  Created by Vitaliy Ivanov on 4/11/12.
//  Copyright (c) 2012 w2f2. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSString (HMAC)

- (NSString*)HMACWithSecret:(NSString*)secret;

@end

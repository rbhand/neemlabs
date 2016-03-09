//
//  W2ClientOrderedParameters.h
//  Trivia
//
//  Created by Vitaliy Ivanov on 7/24/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface W2ClientParameter : NSObject
{
	NSString* key_;
	id value_;
}

+ (W2ClientParameter*)parameterWithKey:(NSString*)key value:(id)value;

- (NSArray*)allValues;

@property (readwrite, strong, nonatomic) NSString* key;
@property (readwrite, strong, nonatomic) id value;

@end


@interface W2ClientOrderedParameters : NSObject
{
	NSMutableArray* array_;
}

- (id)init;
- (id)initWithDictionary:(NSDictionary*)dict;

- (void)setParameterValue:(id)value forKey:(NSString*)key;
- (void)removeParameterForKey:(NSString*)key;
- (id)parameterValueForKey:(NSString*)key;

- (void)append:(W2ClientOrderedParameters*)params;

- (void)removeAllParameters;

@property (readonly, nonatomic) NSArray* array;

@end

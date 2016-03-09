//
//  W2ClientOrderedParameters.m
//  Trivia
//
//  Created by Vitaliy Ivanov on 7/24/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#import "W2ClientOrderedParameters.h"

@implementation W2ClientParameter

@synthesize key = key_, value = value_;

+ (W2ClientParameter*)parameterWithKey:(NSString*)key value:(id)value
{
	W2ClientParameter* param = [[W2ClientParameter alloc] init];
	param.key = key;
	param.value = value;
	return param;
}

- (NSArray*)allValues
{
	return [value_ isKindOfClass:[NSArray class]] ? value_ :
		[NSArray arrayWithObject:value_];
}

@end


@implementation W2ClientOrderedParameters

@synthesize array = array_;

- (id)init
{
	if ((self = [super init]))
	{
		array_ = [[NSMutableArray alloc] init];
	}
	
	return self;
}

- (id)initWithDictionary:(NSDictionary*)dict
{
	if ((self = [super init]))
	{
		array_ = [[NSMutableArray alloc] init];
		
		for (NSString* key in dict)
		{
			if ([key isKindOfClass:[NSString class]])
			{
				[array_ addObject:[W2ClientParameter parameterWithKey:key
					value:[dict objectForKey:key]]];
			}
			else if ([key isKindOfClass:[NSNumber class]])
			{
				[array_ addObject:[W2ClientParameter
					parameterWithKey:[NSString stringWithFormat:@"%@", key]
					value:[dict objectForKey:key]]];
			}
		}
	}
	
	return self;
}

- (void)append:(W2ClientOrderedParameters*)params
{
	for (W2ClientParameter* param in params.array)
	{
		[self setParameterValue:param.value forKey:param.key];
	}
}

- (W2ClientParameter*)parameterForKey:(NSString*)key create:(BOOL)create
{
	for (W2ClientParameter* param in array_)
	{
		if ([param.key isEqualToString:key])
			return param;
	}
	
	if (create)
	{
		W2ClientParameter* param = [W2ClientParameter parameterWithKey:key value:nil];
		[array_ addObject:param];
		return param;
	}
	
	return nil;
}

- (void)setParameterValue:(id)value forKey:(NSString*)key
{
	NSAssert(value, @"value must not be nil");
	[self parameterForKey:key create:YES].value = value;
}

- (void)removeParameterForKey:(NSString*)key
{
	W2ClientParameter* param = [self parameterForKey:key create:NO];
	if (param)
		[array_ removeObject:param];
}

- (id)parameterValueForKey:(NSString*)key
{
	return [self parameterForKey:key create:NO].value;
}

- (void)removeAllParameters
{
	[array_ removeAllObjects];
}

@end

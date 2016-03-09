//
//  W2ClientUtils.m
//  Trivia
//
//  Created by Vitaliy Ivanov on 7/25/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#import "W2ClientUtils.h"

NSDictionary* W2GetDictionaryValue(NSDictionary* from, NSString* key)
{
	NSDictionary* ret = [from objectForKey:key];
	if (![ret isKindOfClass:[NSDictionary class]])
		ret = nil;
	return ret;
}

NSArray* W2GetArrayValue(NSDictionary* from, NSString* key)
{
	NSArray* ret = [from objectForKey:key];
	if (![ret isKindOfClass:[NSArray class]])
		ret = nil;
	return ret;
}

NSString* W2GetStringValue(NSDictionary* from, NSString* key)
{
	NSString* ret = [from objectForKey:key];
	if (![ret isKindOfClass:[NSString class]])
		ret = nil;
	return ret;
}

NSNumber* W2GetNumberValue(NSDictionary* from, NSString* key)
{
	NSNumber* ret = [from objectForKey:key];
	if (![ret isKindOfClass:[NSNumber class]])
		ret = nil;
	return ret;
}

NSString* W2GetStringOrNumberUInt64Value(NSDictionary* from, NSString* key)
{
	NSString* val = W2GetStringValue(from, key);
	if (!val)
	{
		NSNumber* num = W2GetNumberValue(from, key);
		if (num)
			val = [NSString stringWithFormat:@"%llu", [num unsignedLongLongValue]];
	}
	
	return val;
}

NSString* W2URLEscapedString(NSString* string)
{
	CFStringRef cf_str =CFURLCreateStringByAddingPercentEscapes(
		NULL, (__bridge CFStringRef)string, NULL, (CFStringRef)@"!*'\"();:@&=+$,/?%#[]% ",
		kCFStringEncodingUTF8);
	NSString* str = [NSString stringWithString:(__bridge NSString *)cf_str];
	CFRelease(cf_str);
	return str;
}

BOOL W2IsEmailStringValid(NSString* email)
{
	BOOL stricterFilter = NO; // Discussion http://blog.logichigh.com/2010/09/02/validating-an-e-mail-address/
	NSString *stricterFilterString = @"[A-Z0-9a-z._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}";
	NSString *laxString = @".+@.+\\.[A-Za-z]{2}[A-Za-z]*";
	NSString *emailRegex = stricterFilter ? stricterFilterString : laxString;
	NSPredicate *emailTest = [NSPredicate predicateWithFormat:@"SELF MATCHES %@", emailRegex];
	return [emailTest evaluateWithObject:email];
}

NSString* W2TrimmedString(NSString* username)
{
	NSMutableString *mStr = [username mutableCopy];
    CFStringTrimWhitespace((__bridge CFMutableStringRef)mStr);
    NSString *result = [mStr copy];
    return result;
}

NSString* W2CleanPhoneNumber(NSString* phone)
{
	NSCharacterSet *doNotWant = [NSCharacterSet characterSetWithCharactersInString:@" ()-"];
	return [[phone componentsSeparatedByCharactersInSet:doNotWant] componentsJoinedByString:@""];
}

BOOL W2IsUsernameStringValid(NSString* username)
{
	NSString* usernameRegex = @"^[^\\\"^\\\']{5,32}$";
	NSPredicate* usernameTest = [NSPredicate predicateWithFormat:@"SELF MATCHES %@",
		usernameRegex];
	return [usernameTest evaluateWithObject:username];
}

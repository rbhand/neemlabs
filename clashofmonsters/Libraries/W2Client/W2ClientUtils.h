//
//  W2ClientUtils.h
//  Trivia
//
//  Created by Vitaliy Ivanov on 7/25/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifdef __cplusplus
	#define W2_OBJC_EXTERN extern "C"
#else
	#define W2_OBJC_EXTERN
#endif

W2_OBJC_EXTERN NSDictionary* W2GetDictionaryValue(NSDictionary* from, NSString* key);
W2_OBJC_EXTERN NSArray* W2GetArrayValue(NSDictionary* from, NSString* key);
W2_OBJC_EXTERN NSString* W2GetStringValue(NSDictionary* from, NSString* key);
W2_OBJC_EXTERN NSNumber* W2GetNumberValue(NSDictionary* from, NSString* key);
W2_OBJC_EXTERN NSString* W2GetStringOrNumberUInt64Value(NSDictionary* from, NSString* key);

W2_OBJC_EXTERN NSString* W2URLEscapedString(NSString* string);

W2_OBJC_EXTERN BOOL W2IsEmailStringValid(NSString* email);
W2_OBJC_EXTERN NSString* W2TrimmedString(NSString* username);
W2_OBJC_EXTERN NSString* W2CleanPhoneNumber(NSString* phone);
W2_OBJC_EXTERN BOOL W2IsUsernameStringValid(NSString* username);

#if defined(DEBUG) || defined(W2CLIENT_LOGGING)
	#define W2CLog(fmt, ...) NSLog((fmt), ##__VA_ARGS__)
#else
	#define W2CLog(...)
#endif

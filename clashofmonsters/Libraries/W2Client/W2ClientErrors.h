//
//  W2Errors.h
//
//  Created by Vitaliy Ivanov on 7/26/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#import <Foundation/Foundation.h>

#define kW2ClientErrorDomain            @"W2ClientError"

/// Server sent some meaningful error
#define kW2ClientErrorCode_ErrorObject           0x0100
/// Server sent maintenance error
#define kW2ClientErrorCode_Maintenance           0x0200
/// Server responded with HTTP error code, generally this is not an expected behavior
#define kW2ClientErrorCode_HTTPErrorCode         0x0300
/// Server behaved in a way valid server should not normally behave
#define kW2ClientErrorCode_UnexpectedBehavior    0x0400

#define kW2ClientErrorCodeKey                    @"W2ClientErrorCode"

#define kW2ClientRequestOutOfSyncNotification    @"W2ClientRequestOutOfSyncNotification"
#define kW2ClientRequestDoubleLoginNotification  @"W2ClientRequestDoubleLoginNotification"

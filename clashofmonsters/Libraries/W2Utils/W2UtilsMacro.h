//
//  W2UtilsMacro.h
//
//  Created by Vitaliy Ivanov on 9/11/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#define DEFINE_SHARED_INSTANCE_USING_BLOCK(block) \
  static dispatch_once_t pred = 0; \
  __strong static id _sharedObject = nil; \
  dispatch_once(&pred, ^{ \
    _sharedObject = block(); \
  }); \
  return _sharedObject;

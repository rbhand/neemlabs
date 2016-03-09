//
//  W2Utils.m
//  RussianScrabble
//
//  Created by Vitaliy Ivanov on 9/18/12.
//  Copyright (c) 2012 Factorial Complexity. All rights reserved.
//

#import "W2Utils.h"

float rand_float(void)
{
	return (float)arc4random_uniform(0xffffffff) /
		(float)0xffffffff;
}

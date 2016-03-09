//
//  W2Utils.h
//

#import <Foundation/Foundation.h>

float rand_float(void);

#define W2IsIPad   (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
#define W2IsIPhone (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)


//#define W2IsIPhoneRetina (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone && \
//	([[UIScreen mainScreen] respondsToSelector:@selector(scale)] && [[UIScreen mainScreen] scale] == 2))


#define SYSTEM_VERSION_EQUAL_TO(v)                  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedSame)
#define SYSTEM_VERSION_GREATER_THAN(v)              ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedDescending)
#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN(v)                 ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN_OR_EQUAL_TO(v)     ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedDescending)

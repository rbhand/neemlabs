//
//  W2Upgrader.h
//

#import <Foundation/Foundation.h>

@class W2Upgrader;
@protocol W2UpgraderDelegate

- (void)upgrader:(W2Upgrader*)upgrader didFinishWithResult:(BOOL)bought
	error:(NSError*)error;

@end

@class SKProduct;
@interface W2Upgrader : NSObject
{
	__unsafe_unretained id<W2UpgraderDelegate> delegate_;
	NSError* error_;
	BOOL upgradeWasBought_;
	BOOL purchasing_;
	BOOL working_;
	NSString* productId_;
	SKProduct* product_;
}

- (id)initWithProductId:(NSString*)productId;

- (BOOL)canUpgrade;
- (void)upgrade;
- (void)restore;

@property (readonly, assign) BOOL isWorking;
@property (readwrite, assign) id<W2UpgraderDelegate> delegate;

@end

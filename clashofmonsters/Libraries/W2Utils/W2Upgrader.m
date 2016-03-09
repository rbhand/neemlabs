//
//  W2Upgrader.m
//

#import "W2Upgrader.h"
#import <StoreKit/StoreKit.h>

@interface W2Upgrader() <SKPaymentTransactionObserver, SKProductsRequestDelegate>

@end


@implementation W2Upgrader

@synthesize delegate = delegate_;

- (id)initWithProductId:(NSString*)productId
{
	if ((self = [super init]))
	{
		[[SKPaymentQueue defaultQueue] addTransactionObserver:self];
		productId_ = productId;
	}
	
	return self;
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
	working_ = NO;
	error_ = error;
	[delegate_ upgrader:self didFinishWithResult:NO error:error_];
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
	NSLog(@"Products available: %@", response.products);
	NSLog(@"Invalid identifiers: %@", response.invalidProductIdentifiers);
	
	for (SKProduct* product in response.products)
	{
		if ([product.productIdentifier isEqualToString:productId_])
		{
			product_ = product;
		}
	}
	
	if (purchasing_)
		[[SKPaymentQueue defaultQueue] addPayment:[SKPayment paymentWithProduct:product_]];
	else
		[[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

- (BOOL)canUpgrade
{
	return [SKPaymentQueue canMakePayments];
}

- (void)fetchProducts
{
	SKProductsRequest *request = [[SKProductsRequest alloc]
		initWithProductIdentifiers:[NSSet setWithObject:productId_]];
	[request start];
	request.delegate = self;
}

- (void)upgrade
{
	if (working_)
		return;
		
	// if no product id is set just automatically purchase it
	if (![productId_ length])
	{
		dispatch_async(dispatch_get_main_queue(), ^{
			[delegate_ upgrader:self didFinishWithResult:YES error:nil];
		});
		return;
	}
	
	upgradeWasBought_ = NO;
	purchasing_ = YES;
	error_ = nil;
	
	working_ = YES;
	
	if (product_)
		[[SKPaymentQueue defaultQueue] addPayment:[SKPayment
			paymentWithProduct:product_]];
	else
		[self fetchProducts];
}

- (void)restore
{
	if (working_)
		return;
	
	upgradeWasBought_ = NO;
	purchasing_ = NO;
	error_ = nil;
	
	working_ = YES;
	
	if (product_)
		[[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
	else
		[self fetchProducts];
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
	BOOL purchasing_failed = NO;
	
	for (SKPaymentTransaction* transaction in transactions)
	{
		NSLog(@"Transaction: %@ (for %@)", transaction.transactionIdentifier, transaction.payment.productIdentifier);
		switch (transaction.transactionState)
		{
			case SKPaymentTransactionStateFailed	 : NSLog(@"FAILED"); break;
			case SKPaymentTransactionStatePurchasing : NSLog(@"PURCHASING"); break;
			case SKPaymentTransactionStatePurchased	 : NSLog(@"PURCHASED"); break;
			case SKPaymentTransactionStateRestored	 : NSLog(@"RESTORED"); break;
		}
		if (transaction.transactionState == SKPaymentTransactionStateFailed)
		{
			NSLog(@"ERROR: %@", transaction.error.localizedDescription);
		}
		
		if ([transaction.payment.productIdentifier isEqualToString:productId_])
		{
			if (transaction.transactionState == SKPaymentTransactionStatePurchased ||
				transaction.transactionState == SKPaymentTransactionStateRestored)
			{
				upgradeWasBought_ = YES;
				error_ = nil;
			}
			else if (transaction.transactionState == SKPaymentTransactionStateFailed)
			{
				purchasing_failed = YES;
				error_ = transaction.error;
			}
		}
		
		if (transaction.transactionState != SKPaymentTransactionStatePurchasing)
		{
			[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
		}
	}
	
	if (purchasing_failed || upgradeWasBought_)
	{
		working_ = NO;
		[delegate_ upgrader:self didFinishWithResult:upgradeWasBought_ error:error_];
	}
}

- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
	working_ = NO;
	[delegate_ upgrader:self didFinishWithResult:upgradeWasBought_ error:error];
}

- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
	working_ = NO;
	[delegate_ upgrader:self didFinishWithResult:upgradeWasBought_ error:error_];
}

- (BOOL)isWorking
{
	return working_;
}

@end

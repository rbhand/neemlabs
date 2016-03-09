//
//  TutorialViewController.m
//  ClashOfMonsters
//
//  Created by Sergey Gerasimov on 3/15/13.
//  Copyright (c) 2013 Factorial Complexity. All rights reserved.
//

#import "TutorialViewController.h"

@interface TutorialViewController ()

@end

@implementation TutorialViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
//	[webView_ loadRequest:[NSURLRequest requestWithURL:[NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:@"tutorial" ofType:@"html"]]]];

	scrollView_.pagingEnabled = YES;
	scrollView_.contentSize = CGSizeMake(self.view.frame.size.width * 3, scrollView_.frame.size.height);
}

- (int)currentPage
{
	int current_page = round(scrollView_.contentOffset.x / scrollView_.bounds.size.width);
	if (current_page < 0)
		current_page = 0;
	if (current_page > 2)
		current_page = 2;
	return current_page;
}

- (void)updateButtons
{
	btnBack_.enabled = [self currentPage] > 0;
}

- (void)viewWillAppear:(BOOL)animated
{
	[super viewWillAppear:animated];
	scrollView_.contentOffset = CGPointMake(0, 0);
	[self updateButtons];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
	return (toInterfaceOrientation == UIInterfaceOrientationPortrait ||
		toInterfaceOrientation == UIInterfaceOrientationPortraitUpsideDown);
}

- (BOOL)prefersStatusBarHidden
{
	return YES;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations
{
	return UIInterfaceOrientationMaskPortrait|UIInterfaceOrientationMaskPortraitUpsideDown;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (IBAction)back
{
	int current_page = [self currentPage];
	if (current_page > 0)
	{
		[scrollView_ setContentOffset:CGPointMake((current_page-1) * scrollView_.bounds.size.width, 0)
			animated:YES];
	}
}

- (IBAction)next
{
	int current_page = [self currentPage];
	if (current_page == 2)
	{
		cocos2d::CCDirector::sharedDirector()->resume();
		
		[self dismissModalViewControllerAnimated:YES];
	}
	else
	{
		[scrollView_ setContentOffset:CGPointMake((current_page+1) * scrollView_.bounds.size.width, 0)
			animated:YES];
	}
}

- (void)scrollViewDidScroll:(UIScrollView*)sender
{
    // Update the page when more than 50% of the previous/next page is visible
    CGFloat pageWidth = scrollView_.frame.size.width;
    int page = floor((scrollView_.contentOffset.x - pageWidth / 2) / pageWidth) + 1;
    pageControl_.currentPage = page;
	
	[self updateButtons];
}

- (IBAction)changePage
{
    // update the scroll view to the appropriate page
    CGRect frame;
    frame.origin.x = scrollView_.frame.size.width * pageControl_.currentPage;
    frame.origin.y = 0;
    frame.size = scrollView_.frame.size;
    [scrollView_ scrollRectToVisible:frame animated:YES];
}

/*- (void)scrollViewWillBeginDragging:(UIScrollView *)scrollView
{
    pageControlBeingUsed_ = NO;
}
 
- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView
{
    pageControlBeingUsed_ = NO;
}*/

@end

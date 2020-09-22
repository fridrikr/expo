// Copyright © 2018 650 Industries. All rights reserved.

#import <Foundation/Foundation.h>

// Utility protocol helping modules to register with specific platform adapter
// for application lifecycle events.

@protocol EDUMAppLifecycleListener <NSObject>

- (void)onAppBackgrounded;
- (void)onAppForegrounded;

@optional
- (void)onAppContentDidAppear;
- (void)onAppContentWillReload;

@end

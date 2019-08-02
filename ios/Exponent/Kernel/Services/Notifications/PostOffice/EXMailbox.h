//  Copyright © 2019-present 650 Industries. All rights reserved.

#ifndef EXMailbox_h
#define EXMailbox_h

@protocol EXMailbox <NSObject>

- (void)onUserInteraction:(NSBundle*)userInteraction;

- (void)onForegroundNotification:(NSBundle*)notification;

@end

#endif /* EXMailbox_h */

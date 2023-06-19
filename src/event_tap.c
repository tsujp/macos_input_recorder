#include "event_tap.h"

void
event_tap_create(struct event_tap *event_tap, event_tap_callback *callback)
{
    // https://developer.apple.com/documentation/coregraphics/1454426-cgeventtapcreate
    // https://developer.apple.com/documentation/coregraphics/cgeventtaplocation/kcgsessioneventtap
    // kCGSessionEventTap: place event tap at point where HID system (and remote
    // control) events enter a login session.
    // kCGHeadInsertEventTap: insert new event tap before any others at same point.
    // kCGEventTapOptionDefault is 0x00000000 meaning an active filter.
    event_tap->ref = CGEventTapCreate(kCGSessionEventTap,
                                      kCGHeadInsertEventTap,
                                      kCGEventTapOptionDefault,
                                      event_tap->mask, // events of interest
                                      callback,
                                      event_tap // shared data the callback receives
                                      );

    bool event_tap_enabled = event_tap->ref && CGEventTapIsEnabled(event_tap->ref);

    if (event_tap_enabled == false)
    {
        exit(EXIT_FAILURE);
    }

    // Created event tap successfully, create a run loop source for it and add it
    //   to the current run loop.
    event_tap->run_loop_source = CFMachPortCreateRunLoopSource(kCFAllocatorDefault,
                                                               event_tap->ref,
                                                               0);
    CFRunLoopAddSource(CFRunLoopGetMain(), event_tap->run_loop_source, kCFRunLoopCommonModes);

    // Enable event tap.
    CGEventTapEnable(event_tap->ref, true);

    // Execute another run loop which will see our changes (creation of an event
    // tap etc) thus running the event tap.
    CFRunLoopRun();

    // https://developer.apple.com/documentation/corefoundation/cfrunloop?language=objc
}


void
event_tap_destroy(struct event_tap *event_tap)
{
    bool event_tap_enabled = event_tap->ref && CGEventTapIsEnabled(event_tap->ref);

    if (event_tap_enabled)
    {
        CGEventTapEnable(event_tap->ref, false);
        CFMachPortInvalidate(event_tap->ref);
        CFRunLoopRemoveSource(CFRunLoopGetMain(), event_tap->run_loop_source, kCFRunLoopCommonModes);
        CFRelease(event_tap->run_loop_source);
        CFRelease(event_tap->ref);
        event_tap->ref = NULL;
    }
}

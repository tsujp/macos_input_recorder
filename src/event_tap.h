#ifndef MLJQ_EVENT_TAP_H
#define MLJQ_EVENT_TAP_H

#include <Carbon/Carbon.h>

// https://developer.apple.com/documentation/corefoundation/cfmachport?language=objc
// CFMachPortRef, reference to a native Mach port (mach_port_t), communication
// channels the macOS kernel uses. No sending, purpose is to listen to message events
// associated with the Mach port. Create a run loop source with
// CFMachPortCreateRunLoopSource and add it to a run loop CFRunLoopAddSource.
struct event_tap
{
    CFMachPortRef      ref;
    CGEventMask        mask;
    CFRunLoopSourceRef run_loop_source;
};

// https://developer.apple.com/documentation/coregraphics/cgeventtapcallback?language=objc
// Pointer share_data is specified during CGEventTapCreate().
#define EVENT_TAP_CALLBACK(name) \
    CGEventRef name(CGEventTapProxy proxy, \
                    CGEventType type, \
                    CGEventRef event, \
                    void *share_data)

// Creates a typedef'd function with the given name.
typedef EVENT_TAP_CALLBACK(event_tap_callback);

#endif

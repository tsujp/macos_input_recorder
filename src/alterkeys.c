// https://web.archive.org/web/20060716110302/http://www.osxbook.com/book/bonus/
// https://web.archive.org/web/20060716110338/http://www.osxbook.com/book/faq/
// https://web.archive.org/web/20060901044828/http://www.osxbook.com/software/

/*
Receiving, Filtering, and Modifying Key Presses and Releases

The following is an example of programmatically receiving keyboard events (specifically, key-down and key-up events), with the option of filtering and modifying them, on Mac OS X 10.4 and up. 
*/

// alterkeys.c
// http://osxbook.com
//
// Complile using the following command line:
//     gcc -Wall -o alterkeys alterkeys.c -framework ApplicationServices
//
// You need superuser privileges to create the event tap, unless accessibility
// is enabled. To do so, select the "Enable access for assistive devices"
// checkbox in the Universal Access system preference pane.

#include <ApplicationServices/ApplicationServices.h>

// This callback will be invoked every time there is a keystroke.
//
CGEventRef
myCGEventCallback(CGEventTapProxy proxy, CGEventType type,
                  CGEventRef event, void *refcon)
{
    // Paranoid sanity check.
    if ((type != kCGEventKeyDown) && (type != kCGEventKeyUp))
        return event;

    // The incoming keycode.
    CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(
                                       event, kCGKeyboardEventKeycode);

    // Swap 'a' (keycode=0) and 'z' (keycode=6).
    if (keycode == (CGKeyCode)0)
        keycode = (CGKeyCode)6;
    else if (keycode == (CGKeyCode)6)
        keycode = (CGKeyCode)0;

    // Set the modified keycode field in the event.
    CGEventSetIntegerValueField(
        event, kCGKeyboardEventKeycode, (int64_t)keycode);

    // We must return the event for it to be useful.
    return event;
}

int
main(void)
{
    CFMachPortRef      eventTap;
    CGEventMask        eventMask;
    CFRunLoopSourceRef runLoopSource;

    // Create an event tap. We are interested in key presses.
    eventMask = ((1 << kCGEventKeyDown) | (1 << kCGEventKeyUp));
    eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0,
                                eventMask, myCGEventCallback, NULL);
    if (!eventTap) {
        fprintf(stderr, "failed to create event tap\n");
        exit(1);
    }

    // Create a run loop source.
    runLoopSource = CFMachPortCreateRunLoopSource(
                        kCFAllocatorDefault, eventTap, 0);

    // Add to the current run loop.
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource,
                       kCFRunLoopCommonModes);

    // Enable the event tap.
    CGEventTapEnable(eventTap, true);

    // Set it all running.
    CFRunLoopRun();

    // In a real program, one would have arranged for cleaning up.

    exit(0);
}

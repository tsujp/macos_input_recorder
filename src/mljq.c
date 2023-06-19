#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <objc/objc-runtime.h>
//#include <mach/mach_time.h>

#include "log.h"
#include "event_tap.h"

#include "synthesise.c"
#include "event_tap.c"


static struct event_tap event_tap;

struct key_event
{
    uint16_t keycode;
    uint64_t start_time;
    uint64_t end_time;
};

// Very inefficient but let's get this working first and then if it's useful we can
//   optimise the datastructure into something sane.
struct key_event N_KEYS[10]; // Maximum 10 keys at once just because.

uint16_t
get_event_keycode(CGEventRef event)
{
    return (uint16_t)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
}


bool
get_event_autorepeat(CGEventRef event)
{
    return (bool)CGEventGetIntegerValueField(event, kCGKeyboardEventAutorepeat);
}


// TODO: Capture CTRL+C and run event_tap_destroy.
EVENT_TAP_CALLBACK(log_key_events)
{
    // Currently only care about key up and down events, if this changes in future
    //   move back to switch-case.

    // https://developer.apple.com/documentation/coregraphics/1455885-cgeventgetintegervaluefield
    // https://developer.apple.com/documentation/coregraphics/cgeventfield?language=objc
    switch (type) {
    case kCGEventKeyUp: {
        uint16_t keycode = get_event_keycode(event);
        bool is_repeat = get_event_autorepeat(event);

        printf("UP keycode: 0x%.2X, repeat?: %s\n", keycode, is_repeat ? "true" : "false");
        printf(" --> %llu\n", CGEventGetTimestamp(event));
    } break;
    case kCGEventKeyDown: {
        uint16_t keycode = get_event_keycode(event);
        bool is_repeat = get_event_autorepeat(event);

        printf("DOWN keycode: 0x%.2X, repeat?: %s\n", keycode, is_repeat ? "true" : "false");
        printf(" --> %llu\n", CGEventGetTimestamp(event));
        // 1000000
        // fflush(stdout);
    } break;
    }

    fflush(stdout);

    //printf("event!");
    //fflush(stdout);

    return event;
}

int
main(int argc, char **argv)
{
    // verbose = true;
    // debug("skhd: restarting event-tap\n");

    // Not needed I think.
    // mach_timebase_info_data_t base;
    // mach_timebase_info(&base);

    // Event types of interest.
    // https://developer.apple.com/documentation/coregraphics/cgeventtype?language=objc
    event_tap.mask = (1 << kCGEventKeyDown) | (1 << kCGEventKeyUp);
    event_tap_create(&event_tap, log_key_events);









    // // ------------------------- Does work, just commented out.
    // // TODO: Time requested vs. actual sleep time later.
    // printf("Sleep for 2 seconds...\n");

    // struct timespec sleep_for;
    // sleep_for.tv_sec = 0;
    // sleep_for.tv_nsec = (999 * 1000000);

    // struct timespec slept_for;

    // int sleep_res = nanosleep(&sleep_for, &slept_for);

    // printf("...done, code: %d\n", sleep_res);
    // printf("foo: %d\n", errno);

    // printf("Slept for: %ld s, %ld ns\n", slept_for.tv_sec, slept_for.tv_nsec);





    // ----------------- Send some key codes boi

    //synthesise_key(0x02);
    // synthesise_key(kVK_ANSI_D);

    // Hardcoded script lol.

    // First area.
    /*
    synthesise_key_held_for(kVK_RightArrow, 1700);
    synthesise_key(kVK_ANSI_D);

    synthesise_key_held_for(kVK_RightArrow, 1300);
    synthesise_key(kVK_ANSI_D);

    synthesise_key_held_for(kVK_RightArrow, 900);
    synthesise_key(kVK_ANSI_D);

    // jump is about 650 ish ms
    synthesise_key_held_for(kVK_RightArrow, 820);
    synthesise_key(kVK_ANSI_D);

    synthesise_key_held_for(kVK_RightArrow, 820);
    synthesise_key(kVK_ANSI_D);
    */

//     for (int i = 0; i < 50; i++)
// {
    // chair jump glitch
    // synthesise_key_held_for(kVK_ANSI_D, 0);
    // synthesise_key_held_for(kVK_ANSI_0, 600); // 610
    // synthesise_key_held_for(kVK_ANSI_3, 10);
    // synthesise_key_held_for(kVK_ANSI_D, 0);
    

    /*
    synthesise_key_held_for(kVK_ANSI_0, 5);
    synthesise_key_held_for(kVK_LeftArrow, 5);
    synthesise_key_held_for(kVK_LeftArrow, 5);
    synthesise_key_held_for(kVK_ANSI_0, 0);
    synthesise_key_held_for(kVK_ANSI_3, 500);
    */
// }

    return EXIT_SUCCESS;
}

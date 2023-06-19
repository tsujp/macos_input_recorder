#include <Carbon/Carbon.h>

#include "synthesise.h"
#include "math.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated"

static inline void
create_and_post_keyevent(uint16_t key, bool pressed)
{
    CGPostKeyboardEvent((CGCharCode)0, (CGKeyCode)key, pressed);
}

/*
static inline void
synthesize_modifiers(struct hotkey *hotkey, bool pressed)
{
    if (has_flags(hotkey, Hotkey_Flag_Alt)) {
        create_and_post_keyevent(Modifier_Keycode_Alt, pressed);
    }

    if (has_flags(hotkey, Hotkey_Flag_Shift)) {
        create_and_post_keyevent(Modifier_Keycode_Shift, pressed);
    }

    if (has_flags(hotkey, Hotkey_Flag_Cmd)) {
        create_and_post_keyevent(Modifier_Keycode_Cmd, pressed);
    }

    if (has_flags(hotkey, Hotkey_Flag_Control)) {
        create_and_post_keyevent(Modifier_Keycode_Ctrl, pressed);
    }

    if (has_flags(hotkey, Hotkey_Flag_Fn)) {
        create_and_post_keyevent(Modifier_Keycode_Fn, pressed);
    }
}
*/

// If you need to hold a key for more than 65 seconds, tough luck! Hahaha
// If not set will default to 20 milliseconds
void
synthesise_key_held_for(uint16_t key, uint16_t milliseconds)
{
    CGSetLocalEventsSuppressionInterval(0.0f);
    CGEnableEventStateCombining(false);

    struct timespec hold_for;

    if (milliseconds > 999)
    {
        hold_for.tv_sec = (int)floor(milliseconds / 1000);
        hold_for.tv_nsec = (milliseconds - hold_for.tv_sec * 1000) * 1000000;
    }
    else
    {
        hold_for.tv_sec = 0;
        hold_for.tv_nsec = milliseconds * 1000000;
    }

    // Key down, hold, key up.
    create_and_post_keyevent(key, true);
    int sleep_res = nanosleep(&hold_for, NULL); // TODO: Time deltas.
    printf("Pressed [%d] for X ms, ret [%d]\n", key, sleep_res);
    create_and_post_keyevent(key, false);
}

void
synthesise_key(uint16_t key)//char *key_string)
{
    synthesise_key_held_for(key, 20);
}

#pragma clang diagnostic pop

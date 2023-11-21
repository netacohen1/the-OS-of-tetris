#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "util.h"

// io ports
#define KEYBOARD_ENCODER        0x60
#define KEYBOARD_CONTROLLER     0x64

#define KEY_NULL                0
#define KEY_ESC                 0x18        // ascii for escape
#define KEY_BACKSPACE           '\b'        // backspace
#define KEY_TAB                 '\t'        // tab
#define KEY_ENTER               '\n'        // enter -> new line

#define KEY_LCTRL               0x1d
#define KEY_RCTRL               0x1d

#define KEY_LSHIFT              0x2A
#define KEY_RSHIFT              0x36

#define KEY_LALT                0x38
#define KEY_RALT                0x38

#define KEY_CAPS_LOCK           0x3A
#define KEY_NUM_LOCK            0x45
#define KEY_SCROLL_LOCK         0x46

#define KEY_F1                  0x80
#define KEY_F2                  (KEY_F1 + 1)
#define KEY_F3                  (KEY_F1 + 2)
#define KEY_F4                  (KEY_F1 + 3)
#define KEY_F5                  (KEY_F1 + 4)
#define KEY_F6                  (KEY_F1 + 5)
#define KEY_F7                  (KEY_F1 + 6)
#define KEY_F8                  (KEY_F1 + 7)
#define KEY_F9                  (KEY_F1 + 8)
#define KEY_F10                 (KEY_F1 + 9)
#define KEY_F11                 (KEY_F1 + 10)
#define KEY_F12                 (KEY_F1 + 11)

#define KEY_INSERT              0x90
#define KEY_DELETE              0x91
#define KEY_HOME                0x92
#define KEY_END                 0x93
#define KEY_PAGE_UP             0x94
#define KEY_PAFE_DOWN           0x95
#define KEY_LEFT                0x4b        // release - 0xcb
#define KEY_UP                  0x48        // release - 0xc8
#define KEY_RIGHT               0x4D        // release - 0xcd
#define KEY_DOWN                0x50        // release - 0xd0

enum KEYBRD_CTRL_STATUS_BIT{
    OUTPUT_BUFFER_STATUS        = 0x01,
    INPUT_BUFFER_STATUS         = 0x02,
    SYSTEM_STATUS               = 0x04,
    DATA_WHICH_PORT_STATUS      = 0x08,
    LOCKED_SATAUS               = 0x10,
    AUXILIARY_BUFFER_SATAUS     = 0x20,
    TIMEOUT_STATUS              = 0x40,
    PARITY_ERROR_STATUS         = 0x80
};

// keybord layout
extern u8 keybrd_layout[2][128];

// keybrd struct holding important info
struct keybrd {
    bool keysPressed[128];
    u8 mode;
    /* MSB -   DOWN */
    /*         RIGHT */
    /*         UP */
    /*         LEFT */
    /*         CAPS LOCK */
    /*         ALT */
    /*         CTRL */
    /* LSB -   SHIFT */
};

enum KEYBOARD_SPECIAL_KEYS {
    SHIFT              = 0x01,
    CTRL               = 0x02,
    ALT                = 0x04,
    CAPS_LOCK          = 0x08,
    LEFT               = 0x10,
    UP                 = 0x20,
    RIGHT              = 0x40,
    DOWN               = 0x80
};

/* u8 keybrd_ctrl_read_status();           // read status from keyboard controller */
/* void keybrd_ctrl_send_cmd(u8 cmd);      // send command byte to keyboard controller */
/* u8 keybrd_enc_read_buffer();            // read buffer from keyboard encoder */
/* void keybrd_enc_send_cmd(u8 cmd);        // send command to keyboard encoder */

void keybrd_install();                      // register the keyboard handler in irq
bool is_special_key(u8 key);                // check in mode if key is pressed
bool is_key_pressed(u8 c);

void keybrd_set_leds(bool num, bool caps, bool scroll); // turn on/off led lights
                                                        //
#endif

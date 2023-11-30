#include "keyboard.h"
#include "irq.h"
#include "util.h"

u8 keybrd_layout[2][128] = {
    {
        KEY_NULL,KEY_ESC,'1','2','3','4','5','6','7','8','9','0','-','=',KEY_BACKSPACE,
        KEY_TAB,'q','w','e','r','t','y','u','i','o','p','[',']',KEY_ENTER,
        0,'a','s','d','f','g','h','j','k','l',';','\'','`',
        0,'\\','z','x','c','v','b','n','m',',','.','/',0,0,
        0,' ',0,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,
        0,0,0,KEY_UP,0,0,KEY_LEFT,0,KEY_RIGHT,0,0,KEY_DOWN,0,0,0,KEY_F11,KEY_F12
    },
    {
        KEY_NULL,KEY_ESC,'!','@','#','$','%','^','&','*','(',')','_','+',KEY_BACKSPACE,
        KEY_TAB,'Q','W','E','R','T','Y','U','I','O','P','{','}',KEY_ENTER,
        0,'A','S','D','F','G','H','J','K','L',':','"','~',
        0,'|','Z','X','C','V','B','N','M','<','>','?',0,0,
        0,' ',0,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,
        0,0,0,KEY_UP,0,0,KEY_LEFT,0,KEY_RIGHT,0,0,KEY_DOWN,0,0,0,KEY_F11,KEY_F12
    }
};

struct keybrd keybrdState;

void keybrd_handler(Registers_t *regs){
    u8 scanCode = inb(0x60);
    if (scanCode == 0xe0){
        scanCode = inb(0x60);
    } 
    bool releasd = (scanCode > 0x80);       // if key is released -> 1; else -> 0
    switch (scanCode - releasd*0x80){
        case KEY_LSHIFT:
        case KEY_RSHIFT:
            keybrdState.mode = (releasd) ? keybrdState.mode & 0xfe : keybrdState.mode | 1;
            break;
        case KEY_LCTRL: // and RCTRL
            keybrdState.mode = (releasd) ? keybrdState.mode & 0xfd : keybrdState.mode | 2;
            break;
        case KEY_LALT: // and RALT
            keybrdState.mode = (releasd) ? keybrdState.mode & 0xfb : keybrdState.mode | 4;
            break;
        case KEY_CAPS_LOCK:
            keybrdState.mode = (releasd) ? keybrdState.mode : ((keybrdState.mode & 0x8) ? keybrdState.mode & 0xf7 : keybrdState.mode | 8);
            break;
        default:
            if (!releasd){
                keybrdState.keysPressed[keybrd_layout[keybrdState.mode & 1][scanCode]] = 1;

                keybrdState.pressedStack[keybrdState.stackPointer] = keybrd_layout[keybrdState.mode & 1][scanCode];
                keybrdState.stackPointer++;
            } else {
                keybrdState.keysPressed[keybrd_layout[keybrdState.mode & 1][scanCode - RELEASE_MARGIN]] = 0;

            }
        }
}

bool is_special_key(u8 key){
    return keybrdState.mode & key;
}

bool is_key_pressed(u8 key){
    return keybrdState.keysPressed[key];
}

u8 getPressed(){
    u8 key = 0;
    if (keybrdState.stackPointer > 0){
        keybrdState.stackPointer--;
        key = keybrdState.pressedStack[keybrdState.stackPointer];
        keybrdState.pressedStack[keybrdState.stackPointer] = 0;
    }
    return key;
}

void keybrd_install(){
    IRQ_RegisterHandler(1, keybrd_handler);
}

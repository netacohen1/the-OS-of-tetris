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
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,KEY_F11,KEY_F12
    },
    {
        KEY_NULL,KEY_ESC,'!','@','#','$','%','^','&','*','(',')','_','+',KEY_BACKSPACE,
        KEY_TAB,'Q','W','E','R','T','Y','U','I','O','P','{','}',KEY_ENTER,
        0,'A','S','D','F','G','H','J','K','L',':','"','~',
        0,'|','Z','X','C','V','B','N','M','<','>','?',0,0,
        0,' ',0,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,KEY_F11,KEY_F12
    }
};

struct keybrd keybrdState;

// read status from keyboard controller
u8 keybrd_ctrl_read_status(){
    return inb(KEYBOARD_CONTROLLER);
}

// send command byte to keyboard controller
void keybrd_ctrl_send_cmd(u8 cmd){
    while (1)
        if ((keybrd_ctrl_read_status() & INPUT_BUFFER_STATUS) == 0)
            break;
    
    outb(KEYBOARD_CONTROLLER, cmd);
}

// read buffer from keyboard encoder
u8 keybrd_enc_read_buffer(){
    return inb(KEYBOARD_ENCODER);
}

// send command to keyboard encoder
void keybrd_enc_send_cmd(u8 cmd){
    while (1)
        if ((keybrd_ctrl_read_status() & INPUT_BUFFER_STATUS) == 0)
            break;

    outb(KEYBOARD_ENCODER, cmd);
}

// turn on/off led lights
void keybrd_set_leds(bool num, bool caps, bool scroll){
    u8 data = 0;

    data |= (scroll) & 1;
    data |= (num << 2) & 1;
    data |= (caps << 3) & 1;

    /* keybrd_ctrl_send_cmd(KEYBOARD_ENC_CMD_LED); */
    io_wait();
    keybrd_enc_send_cmd(data);
}

void keybrd_setup(){
    keybrd_ctrl_send_cmd(0xf0);
    io_wait();
    QemuPrintf("0x%x\n", inb(0x60));
    /* keybrd_enc_send_cmd(0x00); */
}

void keybrd_handler(Registers_t *regs){
    u8 scanCode = inb(0x60);
    if (scanCode == 0xe0){
        scanCode = inb(0x60);
        bool releasd = (scanCode > 0x80);       // if key is released -> 1
        switch (scanCode - releasd*0x80) {
            case KEY_LEFT:
                keybrdState.mode = (releasd) ? keybrdState.mode & 0xef : keybrdState.mode | 16;
                break;
            case KEY_UP:
                keybrdState.mode = (releasd) ? keybrdState.mode & 0xdf : keybrdState.mode | 32;
                break;
            case KEY_RIGHT:
                keybrdState.mode = (releasd) ? keybrdState.mode & 0xbf : keybrdState.mode | 64;
                break;
            case KEY_DOWN:
                keybrdState.mode = (releasd) ? keybrdState.mode & 0x7f : keybrdState.mode | 128;
                break;
        }
    } else {
        bool releasd = (scanCode > 0x80);       // if key is released -> 1
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
                /* keybrdState.keysPressed[scanCode - releasd*0x80] = (releasd) ? 0 : 1; */
                keybrdState.keysPressed[keybrd_layout[keybrdState.mode & 1][scanCode - releasd*0x80]] = (releasd) ? 0 : 1;
                if (!releasd)
                    QemuPrintf("%c", keybrd_layout[keybrdState.mode&1][scanCode - releasd*0x80]);
            }
    }
}

bool is_special_key(u8 key){
    return keybrdState.mode & key;
}

bool is_key_pressed(u8 c){
    return keybrdState.keysPressed[c];
}

void keybrd_install(){
    /* keybrd_setup(); */
    IRQ_RegisterHandler(1, keybrd_handler);
}

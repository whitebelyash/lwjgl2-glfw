//
// Created by maks on 27.04.2026.
//

#ifndef LWJGL2_LWJGL_EVENT_H
#define LWJGL2_LWJGL_EVENT_H

#define PACKED __attribute__((__packed__))

typedef struct PACKED {
    jint key;
    jbyte state;
    jint character;
    jlong nanos;
	jbyte repeat;
} KeyEvent;

typedef struct PACKED {
    jbyte button;
    jbyte state;
    jint x;
    jint y;
    jint dwheel;
    jlong nanos;
} MouseEvent;

typedef struct PACKED {
	jint x;
	jint y;
	jint dwheel;
} MouseBuffer;

#define PENDING_EVENT_KEY 0
#define PENDING_EVENT_MOUSE 1

typedef struct PACKED {
    jbyte type;
    union {
        KeyEvent k;
        MouseEvent m;
    };
} CompoundEvent;

static inline jint getLwjgl2Keycode(int glfw_key) {
	switch (glfw_key) {
		case GLFW_KEY_ESCAPE: return 0x01;
		case GLFW_KEY_1: return 0x02;
		case GLFW_KEY_2: return 0x03;
		case GLFW_KEY_3: return 0x04;
		case GLFW_KEY_4: return 0x05;
		case GLFW_KEY_5: return 0x06;
		case GLFW_KEY_6: return 0x07;
		case GLFW_KEY_7: return 0x08;
		case GLFW_KEY_8: return 0x09;
		case GLFW_KEY_9: return 0x0A;
		case GLFW_KEY_0: return 0x0B;
		case GLFW_KEY_MINUS: return 0x0C; /* - on main keyboard */
		case GLFW_KEY_EQUAL: return 0x0D;
		case GLFW_KEY_BACKSPACE: return 0x0E; /* backspace */
		case GLFW_KEY_TAB: return 0x0F;
		case GLFW_KEY_Q: return 0x10;
		case GLFW_KEY_W: return 0x11;
		case GLFW_KEY_E: return 0x12;
		case GLFW_KEY_R: return 0x13;
		case GLFW_KEY_T: return 0x14;
		case GLFW_KEY_Y: return 0x15;
		case GLFW_KEY_U: return 0x16;
		case GLFW_KEY_I: return 0x17;
		case GLFW_KEY_O: return 0x18;
		case GLFW_KEY_P: return 0x19;
		case GLFW_KEY_LEFT_BRACKET: return 0x1A;
		case GLFW_KEY_RIGHT_BRACKET: return 0x1B;
		case GLFW_KEY_ENTER: return 0x1C; /* Enter on main keyboard */
		case GLFW_KEY_LEFT_CONTROL: return 0x1D;
		case GLFW_KEY_A: return 0x1E;
		case GLFW_KEY_S: return 0x1F;
		case GLFW_KEY_D: return 0x20;
		case GLFW_KEY_F: return 0x21;
		case GLFW_KEY_G: return 0x22;
		case GLFW_KEY_H: return 0x23;
		case GLFW_KEY_J: return 0x24;
		case GLFW_KEY_K: return 0x25;
		case GLFW_KEY_L: return 0x26;
		case GLFW_KEY_SEMICOLON: return 0x27;
		case GLFW_KEY_APOSTROPHE: return 0x28;
		case GLFW_KEY_GRAVE_ACCENT: return 0x29; /* accent grave */
		case GLFW_KEY_LEFT_SHIFT: return 0x2A;
		case GLFW_KEY_BACKSLASH: return 0x2B;
		case GLFW_KEY_Z: return 0x2C;
		case GLFW_KEY_X: return 0x2D;
		case GLFW_KEY_C: return 0x2E;
		case GLFW_KEY_V: return 0x2F;
		case GLFW_KEY_B: return 0x30;
		case GLFW_KEY_N: return 0x31;
		case GLFW_KEY_M: return 0x32;
		case GLFW_KEY_COMMA: return 0x33;
		case GLFW_KEY_PERIOD: return 0x34; /* . on main keyboard */
		case GLFW_KEY_SLASH: return 0x35; /* / on main keyboard */
		case GLFW_KEY_RIGHT_SHIFT: return 0x36;
		case GLFW_KEY_KP_MULTIPLY: return 0x37; /* * on numeric keypad */
		case GLFW_KEY_LEFT_ALT: return 0x38; /* left Alt */
		case GLFW_KEY_SPACE: return 0x39;
		case GLFW_KEY_CAPS_LOCK: return 0x3A;
		case GLFW_KEY_F1: return 0x3B;
		case GLFW_KEY_F2: return 0x3C;
		case GLFW_KEY_F3: return 0x3D;
		case GLFW_KEY_F4: return 0x3E;
		case GLFW_KEY_F5: return 0x3F;
		case GLFW_KEY_F6: return 0x40;
		case GLFW_KEY_F7: return 0x41;
		case GLFW_KEY_F8: return 0x42;
		case GLFW_KEY_F9: return 0x43;
		case GLFW_KEY_F10: return 0x44;
		case GLFW_KEY_NUM_LOCK: return 0x45;
		case GLFW_KEY_SCROLL_LOCK: return 0x46; /* Scroll Lock */
		case GLFW_KEY_KP_7: return 0x47;
		case GLFW_KEY_KP_8: return 0x48;
		case GLFW_KEY_KP_9: return 0x49;
		case GLFW_KEY_KP_SUBTRACT: return 0x4A; /* - on numeric keypad */
		case GLFW_KEY_KP_4: return 0x4B;
		case GLFW_KEY_KP_5: return 0x4C;
		case GLFW_KEY_KP_6: return 0x4D;
		case GLFW_KEY_KP_ADD: return 0x4E; /* + on numeric keypad */
		case GLFW_KEY_KP_1: return 0x4F;
		case GLFW_KEY_KP_2: return 0x50;
		case GLFW_KEY_KP_3: return 0x51;
		case GLFW_KEY_KP_0: return 0x52;
		case GLFW_KEY_KP_DECIMAL: return 0x53; /* . on numeric keypad */
		case GLFW_KEY_F11: return 0x57;
		case GLFW_KEY_F12: return 0x58;
		case GLFW_KEY_F13: return 0x64; /*                     (NEC PC98) */
		case GLFW_KEY_F14: return 0x65; /*                     (NEC PC98) */
		case GLFW_KEY_F15: return 0x66; /*                     (NEC PC98) */
		case GLFW_KEY_F16: return 0x67; /* Extended Function keys - (Mac) */
		case GLFW_KEY_F17: return 0x68;
		case GLFW_KEY_F18: return 0x69;
		//case GLFW_KEY_KANA: return 0x70; /* (Japanese keyboard)            */
		case GLFW_KEY_F19: return 0x71; /* Extended Function keys - (Mac) */
		//case GLFW_KEY_CONVERT: return 0x79; /* (Japanese keyboard)            */
		//case GLFW_KEY_NOCONVERT: return 0x7B; /* (Japanese keyboard)            */
		//case GLFW_KEY_YEN: return 0x7D; /* (Japanese keyboard)            */
		//case GLFW_KEY_NUMPADEQUALS: return 0x8D; /* = on numeric keypad (NEC PC98) */
		//case GLFW_KEY_CIRCUMFLEX: return 0x90; /* (Japanese keyboard)            */
		//case GLFW_KEY_AT: return 0x91; /*                     (NEC PC98) */
		//case GLFW_KEY_COLON: return 0x92; /*                     (NEC PC98) */
		//case GLFW_KEY_UNDERLINE: return 0x93; /*                     (NEC PC98) */
		//case GLFW_KEY_KANJI: return 0x94; /* (Japanese keyboard)            */
		//case GLFW_KEY_STOP: return 0x95; /*                     (NEC PC98) */
		//case GLFW_KEY_AX: return 0x96; /*                     (Japan AX) */
		//case GLFW_KEY_UNLABELED: return 0x97; /*                        (J3100) */
		case GLFW_KEY_KP_ENTER: return 0x9C; /* Enter on numeric keypad */
		case GLFW_KEY_RIGHT_CONTROL: return 0x9D;
		//case GLFW_KEY_SECTION: return 0xA7; /* Section symbol (Mac) */
		//case GLFW_KEY_NUMPADCOMMA: return 0xB3; /* , on numeric keypad (NEC PC98) */
		case GLFW_KEY_KP_DIVIDE: return 0xB5; /* / on numeric keypad */
		case GLFW_KEY_PRINT_SCREEN: return 0xB7;
		case GLFW_KEY_RIGHT_ALT: return 0xB8; /* right Alt */
		//case GLFW_KEY_FUNCTION: return 0xC4; /* Function (Mac) */
		case GLFW_KEY_PAUSE: return 0xC5; /* Pause */
		case GLFW_KEY_HOME: return 0xC7; /* Home on arrow keypad */
		case GLFW_KEY_UP: return 0xC8; /* UpArrow on arrow keypad */
		//case GLFW_KEY_PRIOR: return 0xC9; /* PgUp on arrow keypad */
		case GLFW_KEY_LEFT: return 0xCB; /* LeftArrow on arrow keypad */
		case GLFW_KEY_RIGHT: return 0xCD; /* RightArrow on arrow keypad */
		case GLFW_KEY_END: return 0xCF; /* End on arrow keypad */
		case GLFW_KEY_DOWN: return 0xD0; /* DownArrow on arrow keypad */
		//case GLFW_KEY_NEXT: return 0xD1; /* PgDn on arrow keypad */
		case GLFW_KEY_INSERT: return 0xD2; /* Insert on arrow keypad */
		case GLFW_KEY_DELETE: return 0xD3; /* Delete on arrow keypad */
		//case GLFW_KEY_CLEAR: return 0xDA; /* Clear key (Mac) */
		case GLFW_KEY_LEFT_SUPER: return 0xDB; /* Left Windows/Option key */
		default:
			return -1;
	}
}

#endif //LWJGL2_LWJGL_EVENT_H
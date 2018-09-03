#include "midi.h"

const unsigned char main_theme[] = { 0x4d, 0x54, 0x68, 0x64, 0x0, 0x0, 0x0, 0x6, 0x0, 0x1, 0x0, 0x2, 0x1, 0xe0, 0x4d, 0x54, 0x72, 0x6b, 0x0, 0x0, 0x8, 0xde, 0x0, 0xff, 0x58, 0x4, 0x4, 0x2, 0x18, 0x8, 0x0, 0xff, 0x59, 0x2, 0x2, 0x0, 0x0, 0xff, 0x51, 0x3, 0x7, 0x81, 0x1c, 0x0, 0xb0, 0x79, 0x0, 0x0, 0xc0, 0x0, 0x0, 0xb0, 0x7, 0x64, 0x0, 0xa, 0x40, 0x0, 0x5b, 0x0, 0x0, 0x5d, 0x0, 0x0, 0xff, 0x21, 0x1, 0x0, 0x0, 0x90, 0x3e, 0x21, 0x0, 0x47, 0x21, 0x0, 0x4a, 0x21, 0x81, 0x63, 0x3e, 0x0, 0x0, 0x47, 0x0, 0x0, 0x4a, 0x0, 0xd, 0x3e, 0x21, 0x81, 0x63, 0x3e, 0x0, 0xd, 0x4a, 0x21, 0x81, 0x63, 0x4a, 0x0, 0xd, 0x3e, 0x21, 0x81, 0x63, 0x3e, 0x0, 0xd, 0x4a, 0x21, 0x81, 0x63, 0x4a, 0x0, 0xd, 0x3e, 0x21, 0x81, 0x63, 0x3e, 0x0, 0xd, 0x4a, 0x21, 0x91, 0x67, 0x4a, 0x0, 0x79, 0x3d, 0x21, 0x0, 0x44, 0x21, 0x0, 0x49, 0x21, 0x81, 0x63, 0x3d, 0x0, 0x0, 0x44, 0x0, 0x0, 0x49, 0x0, 0xd, 0x3d, 0x21, 0x81, 0x63, 0x3d, 0x0, 0xd, 0x49, 0x21, 0x81, 0x63, 0x49, 0x0, 0xd, 0x3d, 0x21, 0x81, 0x63, 0x3d, 0x0, 0xd, 0x49, 0x21, 0x81, 0x63, 0x49, 0x0, 0xd, 0x3d, 0x21, 0x81, 0x63, 0x3d, 0x0, 0xd, 0x49, 0x21, 0x83, 0x47, 0x49, 0x0, 0x19, 0x3e, 0x31, 0x0, 0x4a, 0x31, 0x83, 0x47, 0x3e, 0x0, 0x0, 0x4a, 0x0, 0x19, 0x3f, 0x31, 0x0, 0x4b, 0x31, 0x91, 0x67, 0x3f, 0x0, 0x0, 0x4b, 0x0, 0x79, 0x3e, 0x31, 0x0, 0x4a, 0x31, 0x95, 0x2f, 0x3e, 0x0, 0x0, 0x4a, 0x0, 0x81, 0x11, 0x3e, 0x31, 0x0, 0x4a, 0x31, 0x83, 0x47, 0x3e, 0x0, 0x0, 0x4a, 0x0, 0x19, 0x3f, 0x31, 0x0, 0x4b, 0x31, 0x91, 0x67, 0x3f, 0x0, 0x0, 0x4b, 0x0, 0x79, 0x3e, 0x31, 0x0, 0x4a, 0x31, 0x95, 0x2f, 0x3e, 0x0, 0x0, 0x4a, 0x0, 0x81, 0x11, 0xff, 0x51, 0x3, 0x8, 0x52, 0xb0, 0x0, 0x90, 0x32, 0x40, 0x3b, 0x32, 0x0, 0x3d, 0x32, 0x40, 0x3b, 0x32, 0x0, 0x3d, 0x31, 0x40, 0x77, 0x31, 0x0, 0x79, 0x32, 0x40, 0x77, 0x32, 0x0, 0x79, 0x2f, 0x40, 0x77, 0x2f, 0x0, 0x79, 0x32, 0x40, 0x3b, 0x32, 0x0, 0x3d, 0x32, 0x40, 0x3b, 0x32, 0x0, 0x3d, 0x31, 0x40, 0x77, 0x31, 0x0, 0x79, 0x32, 0x40, 0x77, 0x32, 0x0, 0x79, 0x34, 0x40, 0x77, 0x34, 0x0, 0x79, 0x32, 0x40, 0x3b, 0x32, 0x0, 0x3d, 0x32, 0x40, 0x3b, 0x32, 0x0, 0x3d, 0x31, 0x40, 0x77, 0x31, 0x0, 0x79, 0x32, 0x40, 0x77, 0x32, 0x0, 0x79, 0x2f, 0x40, 0x77, 0x2f, 0x0, 0x79, 0x2a, 0x40, 0x77, 0x2a, 0x0, 0x79, 0x2e, 0x40, 0x77, 0x2e, 0x0, 0x79, 0x2f, 0x4c, 0x81, 0x6f, 0x2f, 0x0, 0x81, 0x71, 0x32, 0x40, 0x0, 0x3e, 0x40, 0x3b, 0x32, 0x0, 0x0, 0x3e, 0x0, 0x3d, 0x32, 0x40, 0x0, 0x3e, 0x40, 0x3b, 0x32, 0x0, 0x0, 0x3e, 0x0, 0x3d, 0x31, 0x40, 0x0, 0x3d, 0x40, 0x77, 0x31, 0x0, 0x0, 0x3d, 0x0, 0x79, 0x32, 0x40, 0x0, 0x3e, 0x40, 0x77, 0x32, 0x0, 0x0, 0x3e, 0x0, 0x79, 0x2f, 0x40, 0x0, 0x3b, 0x40, 0x77, 0x2f, 0x0, 0x0, 0x3b, 0x0, 0x79, 0x32, 0x40, 0x0, 0x3e, 0x40, 0x3b, 0x32, 0x0, 0x0, 0x3e, 0x0, 0x3d, 0x32, 0x40, 0x0, 0x3e, 0x40, 0x3b, 0x32, 0x0, 0x0, 0x3e, 0x0, 0x3d, 0x31, 0x40, 0x0, 0x3d, 0x40, 0x77, 0x31, 0x0, 0x0, 0x3d, 0x0, 0x79, 0x32, 0x40, 0x0, 0x3e, 0x40, 0x77, 0x32, 0x0, 0x0, 0x3e, 0x0, 0x79, 0x34, 0x40, 0x0, 0x40, 0x40, 0x77, 0x34, 0x0, 0x0, 0x40, 0x0, 0x79, 0x32, 0x40, 0x0, 0x3e, 0x40, 0x3b, 0x32, 0x0, 0x0, 0x3e, 0x0, 0x3d, 0x32, 0x40, 0x0, 0x3e, 0x40, 0x3b, 0x32, 0x0, 0x0, 0x3e, 0x0, 0x3d, 0x31, 0x40, 0x0, 0x3d, 0x40, 0x77, 0x31, 0x0, 0x0, 0x3d, 0x0, 0x79, 0x32, 0x40, 0x0, 0x3e, 0x40, 0x77, 0x32, 0x0, 0x0, 0x3e, 0x0, 0x79, 0x2f, 0x40, 0x0, 0x3b, 0x40, 0x77, 0x2f, 0x0, 0x0, 0x3b, 0x0, 0x79, 0x2a, 0x40, 0x0, 0x36, 0x40, 0x77, 0x2a, 0x0, 0x0, 0x36, 0x0, 0x79, 0x2e, 0x40, 0x0, 0x3a, 0x40, 0x77, 0x2e, 0x0, 0x0, 0x3a, 0x0, 0x79, 0x2f, 0x4c, 0x0, 0x3b, 0x4c, 0x81, 0x6f, 0x2f, 0x0, 0x0, 0x3b, 0x0, 0x81, 0x71, 0x32, 0x40, 0x0, 0x39, 0x40, 0x0, 0x3e, 0x40, 0x3b, 0x32, 0x0, 0x0, 0x39, 0x0, 0x0, 0x3e, 0x0, 0x3d, 0x32, 0x40, 0x0, 0x39, 0x40, 0x0, 0x3e, 0x40, 0x3b, 0x32, 0x0, 0x0, 0x39, 0x0, 0x0, 0x3e, 0x0, 0x3d, 0x31, 0x40, 0x0, 0x38, 0x40, 0x0, 0x3d, 0x40, 0x77, 0x31, 0x0, 0x0, 0x38, 0x0, 0x0, 0x3d, 0x0, 0x79, 0x32, 0x40, 0x0, 0x39, 0x40, 0x0, 0x3e, 0x40, 0x77, 0x32, 0x0, 0x0, 0x39, 0x0, 0x0, 0x3e, 0x0, 0x79, 0x34, 0x40, 0x0, 0x3b, 0x40, 0x0, 0x40, 0x40, 0x77, 0x34, 0x0, 0x0, 0x3b, 0x0, 0x0, 0x40, 0x0, 0x79, 0x35, 0x40, 0x0, 0x3c, 0x40, 0x0, 0x41, 0x40, 0x77, 0x35, 0x0, 0x0, 0x3c, 0x0, 0x0, 0x41, 0x0, 0x79, 0x37, 0x40, 0x0, 0x3e, 0x40, 0x0, 0x43, 0x40, 0x77, 0x37, 0x0, 0x0, 0x3e, 0x0, 0x0, 0x43, 0x0, 0x79, 0x35, 0x40, 0x0, 0x3c, 0x40, 0x0, 0x41, 0x40, 0x77, 0x35, 0x0, 0x0, 0x3c, 0x0, 0x0, 0x41, 0x0, 0x79, 0x34, 0x40, 0x0, 0x3b, 0x40, 0x0, 0x40, 0x40, 0x77, 0x34, 0x0, 0x0, 0x3b, 0x0, 0x0, 0x40, 0x0, 0x79, 0x32, 0x40, 0x0, 0x39, 0x40, 0x0, 0x3e, 0x40, 0x3b, 0x32, 0x0, 0x0, 0x39, 0x0, 0x0, 0x3e, 0x0, 0x3d, 0x32, 0x41, 0x0, 0x39, 0x41, 0x0, 0x3e, 0x41, 0x3b, 0x32, 0x0, 0x0, 0x39, 0x0, 0x0, 0x3e, 0x0, 0x3d, 0x31, 0x42, 0x0, 0x38, 0x42, 0x0, 0x3d, 0x42, 0x77, 0x31, 0x0, 0x0, 0x38, 0x0, 0x0, 0x3d, 0x0, 0x79, 0x32, 0x44, 0x0, 0x39, 0x44, 0x0, 0x3e, 0x44, 0x77, 0x32, 0x0, 0x0, 0x39, 0x0, 0x0, 0x3e, 0x0, 0x79, 0x34, 0x46, 0x0, 0x3b, 0x46, 0x0, 0x40, 0x46, 0x77, 0x34, 0x0, 0x0, 0x3b, 0x0, 0x0, 0x40, 0x0, 0x79, 0x35, 0x48, 0x0, 0x3e, 0x48, 0x0, 0x41, 0x48, 0x77, 0x35, 0x0, 0x0, 0x3e, 0x0, 0x0, 0x41, 0x0, 0x79, 0x37, 0x4a, 0x0, 0x3e, 0x4a, 0x0, 0x43, 0x4a, 0x77, 0x37, 0x0, 0x0, 0x3e, 0x0, 0x0, 0x43, 0x0, 0x79, 0x34, 0x5b, 0x0, 0x3d, 0x5b, 0x0, 0x40, 0x5b, 0x81, 0x6f, 0x34, 0x0, 0x0, 0x3d, 0x0, 0x0, 0x40, 0x0, 0x81, 0x71, 0x3e, 0x50, 0x0, 0x45, 0x50, 0x0, 0x4a, 0x50, 0x3b, 0x3e, 0x0, 0x0, 0x45, 0x0, 0x0, 0x4a, 0x0, 0x3d, 0x3e, 0x50, 0x0, 0x45, 0x50, 0x0, 0x4a, 0x50, 0x3b, 0x3e, 0x0, 0x0, 0x45, 0x0, 0x0, 0x4a, 0x0, 0x3d, 0x3d, 0x50, 0x0, 0x44, 0x50, 0x0, 0x49, 0x50, 0x77, 0x3d, 0x0, 0x0, 0x44, 0x0, 0x0, 0x49, 0x0, 0x79, 0x3e, 0x50, 0x0, 0x45, 0x50, 0x0, 0x4a, 0x50, 0x77, 0x3e, 0x0, 0x0, 0x45, 0x0, 0x0, 0x4a, 0x0, 0x79, 0x40, 0x50, 0x0, 0x47, 0x50, 0x0, 0x4c, 0x50, 0x77, 0x40, 0x0, 0x0, 0x47, 0x0, 0x0, 0x4c, 0x0, 0x79, 0x41, 0x50, 0x0, 0x48, 0x50, 0x0, 0x4d, 0x50, 0x77, 0x41, 0x0, 0x0, 0x48, 0x0, 0x0, 0x4d, 0x0, 0x79, 0x43, 0x50, 0x0, 0x4a, 0x50, 0x0, 0x4f, 0x50, 0x77, 0x43, 0x0, 0x0, 0x4a, 0x0, 0x0, 0x4f, 0x0, 0x79, 0x41, 0x50, 0x0, 0x48, 0x50, 0x0, 0x4d, 0x50, 0x77, 0x41, 0x0, 0x0, 0x48, 0x0, 0x0, 0x4d, 0x0, 0x79, 0x40, 0x50, 0x0, 0x47, 0x50, 0x0, 0x4c, 0x50, 0x77, 0x40, 0x0, 0x0, 0x47, 0x0, 0x0, 0x4c, 0x0, 0x79, 0x3e, 0x50, 0x0, 0x45, 0x50, 0x0, 0x4a, 0x50, 0x3b, 0x3e, 0x0, 0x0, 0x45, 0x0, 0x0, 0x4a, 0x0, 0x3d, 0x3e, 0x50, 0x0, 0x45, 0x50, 0x0, 0x4a, 0x50, 0x3b, 0x3e, 0x0, 0x0, 0x45, 0x0, 0x0, 0x4a, 0x0, 0x3d, 0x3d, 0x50, 0x0, 0x44, 0x50, 0x0, 0x49, 0x50, 0x77, 0x3d, 0x0, 0x0, 0x44, 0x0, 0x0, 0x49, 0x0, 0x79, 0x3e, 0x50, 0x0, 0x45, 0x50, 0x0, 0x4a, 0x50, 0x77, 0x3e, 0x0, 0x0, 0x45, 0x0, 0x0, 0x4a, 0x0, 0x79, 0x41, 0x50, 0x0, 0x49, 0x50, 0x0, 0x4d, 0x50, 0x77, 0x41, 0x0, 0x0, 0x49, 0x0, 0x0, 0x4d, 0x0, 0x79, 0x40, 0x50, 0x0, 0x47, 0x50, 0x0, 0x4c, 0x50, 0x1d, 0x40, 0x0, 0x0, 0x47, 0x0, 0x0, 0x4c, 0x0, 0x1f, 0x41, 0x50, 0x38, 0x41, 0x0, 0x4, 0x40, 0x50, 0x38, 0x40, 0x0, 0x4, 0x41, 0x50, 0x38, 0x41, 0x0, 0x4, 0x40, 0x50, 0x81, 0x63, 0x40, 0x0, 0xd, 0x3e, 0x60, 0x0, 0x45, 0x60, 0x0, 0x4a, 0x60, 0x81, 0x6f, 0x3e, 0x0, 0x0, 0x45, 0x0, 0x0, 0x4a, 0x0, 0x81, 0x71, 0x3e, 0x50, 0x0, 0x4a, 0x50, 0x83, 0x47, 0x3e, 0x0, 0x0, 0x4a, 0x0, 0x19, 0x3f, 0x50, 0x0, 0x4b, 0x50, 0x8e, 0x1f, 0x3f, 0x0, 0x0, 0x4b, 0x0, 0x61, 0x3e, 0x50, 0x0, 0x4a, 0x50, 0x8a, 0x57, 0x3e, 0x0, 0x0, 0x4a, 0x0, 0x49, 0x3e, 0x50, 0x0, 0x4a, 0x50, 0x83, 0x47, 0x3e, 0x0, 0x0, 0x4a, 0x0, 0x19, 0x3f, 0x50, 0x0, 0x4b, 0x50, 0x8e, 0x1f, 0x3f, 0x0, 0x0, 0x4b, 0x0, 0x61, 0x3e, 0x50, 0x0, 0x4a, 0x50, 0x8a, 0x57, 0x3e, 0x0, 0x0, 0x4a, 0x0, 0x49, 0xff, 0x51, 0x3, 0xa, 0xc5, 0x59, 0x0, 0x90, 0x26, 0x50, 0x3b, 0x26, 0x0, 0x3d, 0x26, 0x50, 0x3b, 0x26, 0x0, 0x3d, 0x25, 0x50, 0x77, 0x25, 0x0, 0x79, 0x26, 0x50, 0x77, 0x26, 0x0, 0x79, 0x23, 0x50, 0x77, 0x23, 0x0, 0x79, 0x26, 0x50, 0x3b, 0x26, 0x0, 0x3d, 0x26, 0x50, 0x3b, 0x26, 0x0, 0x3d, 0x25, 0x50, 0x77, 0x25, 0x0, 0x79, 0x26, 0x50, 0x77, 0x26, 0x0, 0x79, 0x28, 0x50, 0x77, 0x28, 0x0, 0x79, 0x26, 0x50, 0x3b, 0x26, 0x0, 0x3d, 0x26, 0x50, 0x3b, 0x26, 0x0, 0x3d, 0x25, 0x50, 0x77, 0x25, 0x0, 0x79, 0x26, 0x50, 0x77, 0x26, 0x0, 0x79, 0x23, 0x50, 0x77, 0x23, 0x0, 0x79, 0x1e, 0x50, 0x77, 0x1e, 0x0, 0x79, 0x22, 0x50, 0x77, 0x22, 0x0, 0x79, 0x23, 0x60, 0x83, 0x5f, 0x23, 0x0, 0x1, 0xff, 0x51, 0x3, 0xa, 0x2c, 0x2b, 0x0, 0x90, 0x26, 0x50, 0x0, 0x32, 0x50, 0x3b, 0x26, 0x0, 0x0, 0x32, 0x0, 0x3d, 0x26, 0x50, 0x0, 0x32, 0x50, 0x3b, 0x26, 0x0, 0x0, 0x32, 0x0, 0x3d, 0x25, 0x50, 0x0, 0x31, 0x50, 0x77, 0x25, 0x0, 0x0, 0x31, 0x0, 0x79, 0x26, 0x50, 0x0, 0x32, 0x50, 0x77, 0x26, 0x0, 0x0, 0x32, 0x0, 0x79, 0x23, 0x50, 0x0, 0x2f, 0x50, 0x77, 0x23, 0x0, 0x0, 0x2f, 0x0, 0x79, 0x26, 0x50, 0x0, 0x32, 0x50, 0x3b, 0x26, 0x0, 0x0, 0x32, 0x0, 0x3d, 0x26, 0x50, 0x0, 0x32, 0x50, 0x3b, 0x26, 0x0, 0x0, 0x32, 0x0, 0x3d, 0x25, 0x50, 0x0, 0x31, 0x50, 0x77, 0x25, 0x0, 0x0, 0x31, 0x0, 0x79, 0x26, 0x50, 0x0, 0x32, 0x50, 0x77, 0x26, 0x0, 0x0, 0x32, 0x0, 0x79, 0x28, 0x50, 0x0, 0x34, 0x50, 0x77, 0x28, 0x0, 0x0, 0x34, 0x0, 0x79, 0x26, 0x50, 0x0, 0x32, 0x50, 0x3b, 0x26, 0x0, 0x0, 0x32, 0x0, 0x3d, 0x26, 0x50, 0x0, 0x32, 0x50, 0x3b, 0x26, 0x0, 0x0, 0x32, 0x0, 0x3d, 0x25, 0x50, 0x0, 0x31, 0x50, 0x77, 0x25, 0x0, 0x0, 0x31, 0x0, 0x79, 0x26, 0x50, 0x0, 0x32, 0x50, 0x77, 0x26, 0x0, 0x0, 0x32, 0x0, 0x79, 0x23, 0x50, 0x0, 0x2f, 0x50, 0x77, 0x23, 0x0, 0x0, 0x2f, 0x0, 0x79, 0x1e, 0x50, 0x0, 0x2a, 0x50, 0x77, 0x1e, 0x0, 0x0, 0x2a, 0x0, 0x79, 0x22, 0x50, 0x0, 0x2e, 0x50, 0x77, 0x22, 0x0, 0x0, 0x2e, 0x0, 0x79, 0x23, 0x60, 0x0, 0x2f, 0x60, 0x83, 0x5f, 0x23, 0x0, 0x0, 0x2f, 0x0, 0x1, 0xff, 0x51, 0x3, 0x9, 0xa3, 0x1c, 0x0, 0x90, 0x26, 0x50, 0x0, 0x2d, 0x50, 0x0, 0x32, 0x50, 0x3b, 0x26, 0x0, 0x0, 0x2d, 0x0, 0x0, 0x32, 0x0, 0x3d, 0x26, 0x50, 0x0, 0x2d, 0x50, 0x0, 0x32, 0x50, 0x3b, 0x26, 0x0, 0x0, 0x2d, 0x0, 0x0, 0x32, 0x0, 0x3d, 0x25, 0x50, 0x0, 0x2c, 0x50, 0x0, 0x31, 0x50, 0x77, 0x25, 0x0, 0x0, 0x2c, 0x0, 0x0, 0x31, 0x0, 0x79, 0x26, 0x50, 0x0, 0x2d, 0x50, 0x0, 0x32, 0x50, 0x77, 0x26, 0x0, 0x0, 0x2d, 0x0, 0x0, 0x32, 0x0, 0x79, 0x28, 0x50, 0x0, 0x2f, 0x50, 0x0, 0x34, 0x50, 0x77, 0x28, 0x0, 0x0, 0x2f, 0x0, 0x0, 0x34, 0x0, 0x79, 0x29, 0x50, 0x0, 0x30, 0x50, 0x0, 0x35, 0x50, 0x77, 0x29, 0x0, 0x0, 0x30, 0x0, 0x0, 0x35, 0x0, 0x79, 0x2b, 0x50, 0x0, 0x32, 0x50, 0x0, 0x37, 0x50, 0x77, 0x2b, 0x0, 0x0, 0x32, 0x0, 0x0, 0x37, 0x0, 0x79, 0x29, 0x50, 0x0, 0x30, 0x50, 0x0, 0x35, 0x50, 0x77, 0x29, 0x0, 0x0, 0x30, 0x0, 0x0, 0x35, 0x0, 0x79, 0x28, 0x50, 0x0, 0x2f, 0x50, 0x0, 0x34, 0x50, 0x77, 0x28, 0x0, 0x0, 0x2f, 0x0, 0x0, 0x34, 0x0, 0x79, 0x26, 0x50, 0x0, 0x2d, 0x50, 0x0, 0x32, 0x50, 0x3b, 0x26, 0x0, 0x0, 0x2d, 0x0, 0x0, 0x32, 0x0, 0x3d, 0x26, 0x50, 0x0, 0x2d, 0x50, 0x0, 0x32, 0x50, 0x3b, 0x26, 0x0, 0x0, 0x2d, 0x0, 0x0, 0x32, 0x0, 0x3d, 0x25, 0x50, 0x0, 0x2c, 0x50, 0x0, 0x31, 0x50, 0x77, 0x25, 0x0, 0x0, 0x2c, 0x0, 0x0, 0x31, 0x0, 0x79, 0x26, 0x50, 0x0, 0x2d, 0x50, 0x0, 0x32, 0x50, 0x77, 0x26, 0x0, 0x0, 0x2d, 0x0, 0x0, 0x32, 0x0, 0x79, 0x28, 0x50, 0x0, 0x2f, 0x50, 0x0, 0x34, 0x50, 0x77, 0x28, 0x0, 0x0, 0x2f, 0x0, 0x0, 0x34, 0x0, 0x79, 0x29, 0x50, 0x0, 0x32, 0x50, 0x0, 0x35, 0x50, 0x77, 0x29, 0x0, 0x0, 0x32, 0x0, 0x0, 0x35, 0x0, 0x79, 0x2b, 0x50, 0x0, 0x32, 0x50, 0x0, 0x37, 0x50, 0x77, 0x2b, 0x0, 0x0, 0x32, 0x0, 0x0, 0x37, 0x0, 0x79, 0x28, 0x60, 0x0, 0x31, 0x60, 0x0, 0x34, 0x60, 0x81, 0x6f, 0x28, 0x0, 0x0, 0x31, 0x0, 0x0, 0x34, 0x0, 0x81, 0x71, 0xff, 0x51, 0x3, 0x9, 0x27, 0xbf, 0x0, 0x90, 0x32, 0x50, 0x0, 0x39, 0x50, 0x0, 0x3e, 0x50, 0x3b, 0x32, 0x0, 0x0, 0x39, 0x0, 0x0, 0x3e, 0x0, 0x3d, 0x32, 0x50, 0x0, 0x39, 0x50, 0x0, 0x3e, 0x50, 0x3b, 0x32, 0x0, 0x0, 0x39, 0x0, 0x0, 0x3e, 0x0, 0x3d, 0x31, 0x50, 0x0, 0x38, 0x50, 0x0, 0x3d, 0x50, 0x77, 0x31, 0x0, 0x0, 0x38, 0x0, 0x0, 0x3d, 0x0, 0x79, 0x32, 0x50, 0x0, 0x39, 0x50, 0x0, 0x3e, 0x50, 0x77, 0x32, 0x0, 0x0, 0x39, 0x0, 0x0, 0x3e, 0x0, 0x79, 0x34, 0x50, 0x0, 0x3b, 0x50, 0x0, 0x40, 0x50, 0x77, 0x34, 0x0, 0x0, 0x3b, 0x0, 0x0, 0x40, 0x0, 0x79, 0x35, 0x50, 0x0, 0x3c, 0x50, 0x0, 0x41, 0x50, 0x77, 0x35, 0x0, 0x0, 0x3c, 0x0, 0x0, 0x41, 0x0, 0x79, 0x37, 0x50, 0x0, 0x3e, 0x50, 0x0, 0x43, 0x50, 0x77, 0x37, 0x0, 0x0, 0x3e, 0x0, 0x0, 0x43, 0x0, 0x79, 0x35, 0x50, 0x0, 0x3c, 0x50, 0x0, 0x41, 0x50, 0x77, 0x35, 0x0, 0x0, 0x3c, 0x0, 0x0, 0x41, 0x0, 0x79, 0x34, 0x50, 0x0, 0x3b, 0x50, 0x0, 0x40, 0x50, 0x77, 0x34, 0x0, 0x0, 0x3b, 0x0, 0x0, 0x40, 0x0, 0x79, 0x32, 0x50, 0x0, 0x39, 0x50, 0x0, 0x3e, 0x50, 0x3b, 0x32, 0x0, 0x0, 0x39, 0x0, 0x0, 0x3e, 0x0, 0x3d, 0x32, 0x50, 0x0, 0x39, 0x50, 0x0, 0x3e, 0x50, 0x3b, 0x32, 0x0, 0x0, 0x39, 0x0, 0x0, 0x3e, 0x0, 0x3d, 0x31, 0x50, 0x0, 0x38, 0x50, 0x0, 0x3d, 0x50, 0x77, 0x31, 0x0, 0x0, 0x38, 0x0, 0x0, 0x3d, 0x0, 0x79, 0x32, 0x50, 0x0, 0x39, 0x50, 0x0, 0x3e, 0x50, 0x77, 0x32, 0x0, 0x0, 0x39, 0x0, 0x0, 0x3e, 0x0, 0x79, 0x35, 0x50, 0x0, 0x3d, 0x50, 0x0, 0x41, 0x50, 0x77, 0x35, 0x0, 0x0, 0x3d, 0x0, 0x0, 0x41, 0x0, 0x79, 0x34, 0x50, 0x0, 0x3b, 0x50, 0x0, 0x40, 0x50, 0x1d, 0x34, 0x0, 0x0, 0x3b, 0x0, 0x0, 0x40, 0x0, 0x1f, 0x35, 0x50, 0x38, 0x35, 0x0, 0x4, 0x34, 0x50, 0x38, 0x34, 0x0, 0x4, 0x35, 0x50, 0x38, 0x35, 0x0, 0x4, 0x34, 0x50, 0x81, 0x63, 0x34, 0x0, 0xd, 0x32, 0x60, 0x0, 0x39, 0x60, 0x0, 0x3e, 0x60, 0x91, 0x7f, 0x32, 0x0, 0x0, 0x39, 0x0, 0x0, 0x3e, 0x0, 0x1, 0xff, 0x2f, 0x0, 0x4d, 0x54, 0x72, 0x6b, 0x0, 0x0, 0x4, 0x4c, 0x0, 0xff, 0x59, 0x2, 0x2, 0x0, 0x0, 0xff, 0x21, 0x1, 0x0, 0x0, 0x90, 0x26, 0x21, 0x0, 0x32, 0x21, 0x2, 0xb0, 0x40, 0x7f, 0x8e, 0x7f, 0x40, 0x0, 0x1, 0x40, 0x7f, 0x8d, 0x3d, 0x90, 0x26, 0x0, 0x0, 0x32, 0x0, 0x81, 0x41, 0x25, 0x21, 0x0, 0x31, 0x21, 0x1, 0xb0, 0x40, 0x0, 0x1, 0x40, 0x7f, 0x8e, 0x1d, 0x90, 0x25, 0x0, 0x0, 0x31, 0x0, 0x62, 0xb0, 0x40, 0x0, 0x1, 0x40, 0x7f, 0x83, 0x5e, 0x90, 0x33, 0x31, 0x81, 0x6f, 0x33, 0x0, 0x81, 0x71, 0x27, 0x31, 0x81, 0x6f, 0x27, 0x0, 0x81, 0x71, 0x33, 0x31, 0x81, 0x6f, 0x33, 0x0, 0x81, 0x71, 0x27, 0x31, 0x1, 0xb0, 0x40, 0x0, 0x1, 0x40, 0x7f, 0x81, 0x6d, 0x90, 0x27, 0x0, 0x81, 0x71, 0x33, 0x31, 0x81, 0x6f, 0x33, 0x0, 0x81, 0x71, 0x32, 0x31, 0x87, 0x41, 0xb0, 0x40, 0x0, 0x1, 0x40, 0x7f, 0x8d, 0x6d, 0x90, 0x32, 0x0, 0x81, 0x12, 0xb0, 0x40, 0x0, 0x1, 0x40, 0x7f, 0x83, 0x5e, 0x90, 0x33, 0x31, 0x81, 0x6f, 0x33, 0x0, 0x81, 0x71, 0x27, 0x31, 0x81, 0x6f, 0x27, 0x0, 0x81, 0x71, 0x33, 0x31, 0x81, 0x6f, 0x33, 0x0, 0x81, 0x71, 0x27, 0x31, 0x1, 0xb0, 0x40, 0x0, 0x1, 0x40, 0x7f, 0x81, 0x6d, 0x90, 0x27, 0x0, 0x81, 0x71, 0x33, 0x31, 0x81, 0x6f, 0x33, 0x0, 0x81, 0x71, 0x32, 0x31, 0x87, 0x41, 0xb0, 0x40, 0x0, 0x1, 0x40, 0x7f, 0x8d, 0x6d, 0x90, 0x32, 0x0, 0x81, 0x11, 0x26, 0x40, 0x1, 0xb0, 0x40, 0x0, 0x81, 0x6e, 0x90, 0x26, 0x0, 0x81, 0x71, 0x26, 0x40, 0x81, 0x6f, 0x26, 0x0, 0x81, 0x71, 0x26, 0x40, 0x81, 0x6f, 0x26, 0x0, 0x81, 0x71, 0x26, 0x40, 0x81, 0x6f, 0x26, 0x0, 0x81, 0x71, 0x26, 0x40, 0x81, 0x6f, 0x26, 0x0, 0x81, 0x71, 0x26, 0x40, 0x81, 0x6f, 0x26, 0x0, 0x81, 0x71, 0x26, 0x40, 0x81, 0x6f, 0x26, 0x0, 0x81, 0x71, 0x26, 0x40, 0x81, 0x6f, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x40, 0x0, 0x26, 0x40, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x40, 0x0, 0x26, 0x40, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x40, 0x0, 0x26, 0x40, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x40, 0x0, 0x26, 0x40, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x40, 0x0, 0x26, 0x40, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x40, 0x0, 0x26, 0x40, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x40, 0x0, 0x26, 0x40, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x40, 0x0, 0x26, 0x40, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x40, 0x0, 0x26, 0x40, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x40, 0x0, 0x26, 0x40, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x40, 0x0, 0x26, 0x40, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x40, 0x0, 0x26, 0x40, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x40, 0x0, 0x26, 0x40, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x44, 0x0, 0x26, 0x44, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x48, 0x0, 0x26, 0x48, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x4c, 0x0, 0x26, 0x4c, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x50, 0x0, 0x26, 0x50, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x50, 0x0, 0x26, 0x50, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x50, 0x0, 0x26, 0x50, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x50, 0x0, 0x26, 0x50, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x50, 0x0, 0x26, 0x50, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x50, 0x0, 0x26, 0x50, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x50, 0x0, 0x26, 0x50, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x81, 0x71, 0x1a, 0x50, 0x0, 0x26, 0x50, 0x81, 0x6f, 0x1a, 0x0, 0x0, 0x26, 0x0, 0x85, 0x51, 0x27, 0x50, 0x77, 0x27, 0x0, 0x79, 0x33, 0x50, 0x77, 0x33, 0x0, 0x79, 0x27, 0x50, 0x77, 0x27, 0x0, 0x79, 0x33, 0x50, 0x77, 0x33, 0x0, 0x79, 0x27, 0x50, 0x77, 0x27, 0x0, 0x79, 0x33, 0x50, 0x77, 0x33, 0x0, 0x79, 0x27, 0x50, 0x77, 0x27, 0x0, 0x79, 0x33, 0x50, 0x77, 0x33, 0x0, 0x79, 0x26, 0x50, 0x77, 0x26, 0x0, 0x79, 0x32, 0x50, 0x77, 0x32, 0x0, 0x79, 0x26, 0x50, 0x77, 0x26, 0x0, 0x79, 0x32, 0x50, 0x77, 0x32, 0x0, 0x79, 0x26, 0x50, 0x77, 0x26, 0x0, 0x79, 0x32, 0x50, 0x77, 0x32, 0x0, 0x79, 0x26, 0x50, 0x77, 0x26, 0x0, 0x79, 0x32, 0x50, 0x77, 0x32, 0x0, 0x79, 0x27, 0x50, 0x77, 0x27, 0x0, 0x79, 0x33, 0x50, 0x77, 0x33, 0x0, 0x79, 0x27, 0x50, 0x77, 0x27, 0x0, 0x79, 0x33, 0x50, 0x77, 0x33, 0x0, 0x79, 0x27, 0x50, 0x77, 0x27, 0x0, 0x79, 0x33, 0x50, 0x77, 0x33, 0x0, 0x79, 0x27, 0x50, 0x77, 0x27, 0x0, 0x79, 0x33, 0x50, 0x77, 0x33, 0x0, 0x79, 0x26, 0x50, 0x77, 0x26, 0x0, 0x79, 0x32, 0x50, 0x77, 0x32, 0x0, 0x79, 0x26, 0x50, 0x77, 0x26, 0x0, 0x79, 0x32, 0x50, 0x77, 0x32, 0x0, 0x79, 0x26, 0x50, 0x77, 0x26, 0x0, 0x79, 0x32, 0x50, 0x77, 0x32, 0x0, 0x79, 0x1a, 0x50, 0x77, 0x1a, 0x0, 0x79, 0x17, 0x50, 0x77, 0x17, 0x0, 0x79, 0x1a, 0x50, 0x77, 0x1a, 0x0, 0x79, 0x17, 0x50, 0x77, 0x17, 0x0, 0x79, 0x1a, 0x50, 0x77, 0x1a, 0x0, 0x79, 0x17, 0x50, 0x77, 0x17, 0x0, 0x79, 0x1a, 0x50, 0x77, 0x1a, 0x0, 0x79, 0x17, 0x50, 0x77, 0x17, 0x0, 0x79, 0x1a, 0x50, 0x77, 0x1a, 0x0, 0x79, 0x17, 0x50, 0x77, 0x17, 0x0, 0x79, 0x1a, 0x50, 0x77, 0x1a, 0x0, 0x79, 0x17, 0x50, 0x77, 0x17, 0x0, 0x79, 0x1a, 0x50, 0x77, 0x1a, 0x0, 0x79, 0x17, 0x50, 0x77, 0x17, 0x0, 0x79, 0x1a, 0x50, 0x77, 0x1a, 0x0, 0x79, 0x17, 0x50, 0x77, 0x17, 0x0, 0x79, 0x1a, 0x50, 0x77, 0x1a, 0x0, 0x79, 0x17, 0x50, 0x77, 0x17, 0x0, 0x79, 0x1a, 0x50, 0x77, 0x1a, 0x0, 0x79, 0x17, 0x50, 0x77, 0x17, 0x0, 0x79, 0x1a, 0x50, 0x77, 0x1a, 0x0, 0x79, 0x17, 0x50, 0x77, 0x17, 0x0, 0x79, 0x1a, 0x50, 0x77, 0x1a, 0x0, 0x79, 0x17, 0x50, 0x77, 0x17, 0x0, 0x79, 0x1a, 0x50, 0x77, 0x1a, 0x0, 0x79, 0x17, 0x50, 0x77, 0x17, 0x0, 0x79, 0x1a, 0x50, 0x77, 0x1a, 0x0, 0x79, 0x17, 0x50, 0x77, 0x17, 0x0, 0x79, 0x1a, 0x50, 0x77, 0x1a, 0x0, 0x79, 0x17, 0x50, 0x77, 0x17, 0x0, 0x79, 0x1a, 0x50, 0x77, 0x1a, 0x0, 0x79, 0x17, 0x50, 0x77, 0x17, 0x0, 0x79, 0x26, 0x50, 0x77, 0x26, 0x0, 0x82, 0x69, 0x26, 0x50, 0x77, 0x26, 0x0, 0x82, 0x69, 0x26, 0x50, 0x77, 0x26, 0x0, 0x82, 0x69, 0x26, 0x50, 0x77, 0x26, 0x0, 0x82, 0x69, 0x26, 0x50, 0x77, 0x26, 0x0, 0x82, 0x69, 0x26, 0x50, 0x77, 0x26, 0x0, 0x82, 0x69, 0x26, 0x50, 0x77, 0x26, 0x0, 0x82, 0x69, 0x26, 0x50, 0x77, 0x26, 0x0, 0x82, 0x69, 0x26, 0x50, 0x77, 0x26, 0x0, 0x82, 0x69, 0x26, 0x50, 0x77, 0x26, 0x0, 0x82, 0x69, 0x26, 0x50, 0x77, 0x26, 0x0, 0x82, 0x69, 0x26, 0x50, 0x77, 0x26, 0x0, 0x82, 0x69, 0x26, 0x50, 0x77, 0x26, 0x0, 0x82, 0x69, 0x26, 0x50, 0x77, 0x26, 0x0, 0x82, 0x69, 0x26, 0x50, 0x77, 0x26, 0x0, 0x82, 0x69, 0x26, 0x50, 0x91, 0x67, 0x26, 0x0, 0x1, 0xff, 0x2f, 0x0, 0xff };

const unsigned int main_theme_length = 3401;

/* 
 * Plays a specified MIDI file by using MCI_OPEN and MCI_PLAY. Returns 
 * as soon as playback begins. The window procedure function for the 
 * specified window will be notified when playback is complete. 
 * Returns 0L on success; otherwise, it returns an MCI error code. 
 */
DWORD playMIDIFile(HWND hWndNotify, LPSTR lpszMIDIFileName)
{
	UINT wDeviceID;
	DWORD dwReturn;
	MCI_OPEN_PARMS mciOpenParms;
	MCI_PLAY_PARMS mciPlayParms;
	MCI_STATUS_PARMS mciStatusParms;
	MCI_SEQ_SET_PARMS mciSeqSetParms;

	// Open the device by specifying the device and filename.
	// MCI will attempt to choose the MIDI mapper as the output port.
	mciOpenParms.lpstrDeviceType = "sequencer";
	mciOpenParms.lpstrElementName = lpszMIDIFileName;
	if (dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpenParms))
	{
		// Failed to open device. Don't close it; just return error.
		return (dwReturn);
	}

	// The device opened successfully; get the device ID.
	wDeviceID = mciOpenParms.wDeviceID;

	// Check if the output port is the MIDI mapper.
	mciStatusParms.dwItem = MCI_SEQ_STATUS_PORT;
	if (dwReturn = mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatusParms))
	{
		mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
		return (dwReturn);
	}

	// The output port is not the MIDI mapper. 
	// Ask if the user wants to continue.
	//if (LOWORD(mciStatusParms.dwReturn) != MIDI_MAPPER)
	//{
		// User does not want to continue. Not an error;
		// just close the device and return.
		//mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
		//return (0L);
	//}

	// Begin playback. The window procedure function for the parent 
	// window will be notified with an MM_MCINOTIFY message when 
	// playback is complete. At this time, the window procedure closes 
	// the device.
	mciPlayParms.dwCallback = (DWORD) hWndNotify;
	if (dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlayParms))
	{
		mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
		return (dwReturn);
	}

	return (0L);
}

char *initMIDIfile()
{
	char *tempPath = new char[L_tmpnam];
	std::tmpnam(tempPath);
	FILE *fMIDI = fopen(tempPath, "wb");
	for (unsigned int i = 0; i < main_theme_length; i++) 
	{
		fputc(main_theme[i], fMIDI);
	}
	fflush(fMIDI);
	fclose(fMIDI);
	return tempPath;
}
# N-key Roll Over (NKRO) - WIP

A standard "BOOT compatible" HID keyboard can send at most 6 keys per HID report. 
This is explained in the parent READMEs of this repo. 
While these keyboards provide the advantage that they can be used in BIOS, they don't offer optimal performance for those wanting large simultaneous key presses (gamers I'm looking at you). 
To achieve NKRO the keyboards HID descriptor must be changed to structure the keypresses as bit flags instead of byte values.

This means that sending an 'a' press is no longer putting the `0x04` value into one of the report's 6 key bytes but instead you would set the 4th bit in the bitmap that has the same length as the largest keycode you wish to support.

As such, the individual report will be much larger as they will require a bitmap that is, on average, 104+ bits in length. 
Meaning that using 14+ bytes for the report is a smart idea. 
Given that you need 1 byte to represent the modifiers (this part does not change), you can at most represent `(${REPORT_LENGTH} - 1) * 8` different key codes.

An example HID descriptor for a HID report is as follows

``` c
#define HID_REPORT_BYTES 14
struct NKRO_HID {
// KEYBOARD 57 bytes
0x05, 0x01,     //  USAGE_PAGE(generic desktop)
0x09, 0x06,     //  USAGE(keyboard)
0xA1, 0x01,     //  COLLECTION(application)
0x05, 0x07,     //  USAGE_PAGE(keyboard)
// modifiers
0x75, 0x01,     //      REPORT_SIZE(1)
0x95, 0x08,     //      REPORT_COUNT(8)
0x19, 0xE0,     //      USAGE_MINIMUM(left control)
0x29, 0xE7,     //      USAGE_MAXIMUM(right gui)
0x15, 0x00,     //      LOGICAL_MINIMUM(0)
0x25, 0x01,     //      LOGICAL_MAXIMUM(1)
0x81, 0x02,     //      INPUT(data, var, abs)
// LEDs
0x05, 0x08,     //      USAGE_PAGE(LEDs)
0x75, 0x01,     //      REPORT_SIZE(1)
0x95, 0x05,     //      REPORT_COUNT(5)
0x19, 0x01,     //      USAGE_MINIMUM(num lock)
0x29, 0x05,     //      USAGE_MAXIMUM(Kana)
0x91, 0x02,     //      OUTPUT(data, var, abs)
0x75, 0x03,     //      REPORT_SIZE(3)
0x95, 0x01,     //      REPORT_COUNT(1)
0x91, 0x01,     //      OUTPUT(const, var, abs)  **padding**
// Keycodes
0x05, 0x07,     //      USAGE_PAGE(keycodes)
0x19, 0x00,     //      USAGE_MINIMUM(0)
0x29, (HID_REPORT_BYTES - 1) * 8 - 1,     //      USAGE_MAXIMUM(101)
0x95, (HID_REPORT_BYTES - 1) * 8,     //      REPORT_COUNT(6)
0x75, 0x01,     //      REPORT_SIZE(8)
0x15, 0x00,     //      LOGICAL_MINIMUM(0)
0x26, 0xE7,     //      LOGICAL_MAXIMUM(231)

0x81, 0x02,     //      INPUT(data, var, abs)
0xC0,           //  END_COLLECTION
// Total bytes = 57
};
```

Now this leaves us with the problem that our keyboard is no longer "BOOT" compatible.
Reading [this]("https://www.devever.net/~hl/usbnkro") very enlightening write up it seems that there is a very easy trick that one can do to mitigate this problem.
To sumarize, during BOOT the PC does not read device descriptors as this is done by the operating system which is not loaded at boot time. 
This is why BOOT keyboards must follow the predefined device descriptor, seen [here](https://www.usb.org/sites/default/files/documents/hid1_11.pdf#page=69).

Quoting:
> The HID Subclass 1 defines two descriptors for Boot Devices. Devices may append additional data to these boot reports, but the first 8 bytes of keyboard reports and the first 3 bytes of mouse reports must conform to the format defined by the Boot Report descriptor in order for the data to be correctly interpreted by the BIOS. The report may not exceed 8 bytes in length. The BIOS will ignore any extensions to reports.

As such we can structure out device descriptor to have the following rough structure:

- Modifier byte
- Reserved byte required for BOOT
- 6 Bytes that will be fill with 6-key roll over (6KRO) values
- NKRO bitmap that will be used by OS

The magic here is the during BOOT the 6KRO values will be read and once the OS is loaded and the descriptor is loaded then these values will be ignored as padding.
We will of course have to always fill these 6KRO values with values to make sure the keyboard works during boot but given the power of boards such as the bluepill this shouldn't be a problem.

Thus we can prepend a boot keyboard's descriptor ([here](https://www.usb.org/sites/default/files/documents/hid1_11.pdf#page=79)), making our complete descriptor something like

``` c
// Keyboard - 8 bytes
0x05, 0x01,     //  USAGE_PAGE(generic desktop)
0x09, 0x06,     //  USAGE(keyboard)
0xA1, 0x01,     //  COLLECTION(application)
0x05, 0x07,     //      USAGE_PAGE(key codes)
// modifier byte - 14 bytes
0x19, 0xE0,     //      USAGE_MINIMUM(left control)
0x29, 0xE7,     //      USAGE_MAXIMUM(right gui)
0x15, 0x00,     //      LOGICAL_MINIMUM(0)
0x25, 0x01,     //      LOGICAL_MAXIMUM(1)
0x75, 0x01,     //      REPORT_SIZE(1)
0x95, 0x08,     //      REPORT_COUNT(8)
0x81, 0x02,     //      INPUT(data, var, abs)
// reserved byte - 6 bytes
0x95, 0x01,     //      REPORT_COUNT(1)
0x75, 0x08,     //      REPORT_SIZE(8)
0x81, 0x01,     //      INPUT(constant)

// 6KRO padding - 16 bytes
0x95, 0x06,     //      REPORT_COUNT(6)
0x75, 0x08,     //      REPORT_SIZE(8)
0x15, 0x00,     //      LOGICAL_MINIMUM(0)
0x25, 0x65,     //      LOGICAL_MAXIMUM(101)
0x05, 0x07,     //      USAGE_PAGE(key codes)
0x19, 0x01,     //      USAGE_MINIMUM(0)
0x29, 0x05,     //      USAGE_MAXIMUM(101)
0x81, 0x01,     //      INPUT(const, array, abs) <- PADDING
//0x81, 0x00,     //      INPUT(const, array, abs) <- Standard
// Wrapping up - 1 byte
0xC0,           //  END_COLLECTION
//  NKRO Keyboard - 8 bytes
0x05, 0x01,     //  USAGE_PAGE (Generic Desktop)
0x09, 0x06,     //  USAGE (Keyboard)
0xa1, 0x01,     //  COLLECTION (Application)
0x05, 0x07,     //      USAGE_PAGE (Keyboard)
// NKRO bitmap - 16 bytes
0x05, 0x07,     //      USAGE_PAGE (keycodes) 
0x19, 0x00,		//      Usage Minimum (0)
0x29, (HID_REPORT_BYTES - 1) * 8 - 1,	// Usage Maximum (103 with 14 byte report)
0x15, 0x00,		//      Logical Minimum (0)
0x25, 0x01,		//      Logical Maximum (1)
0x75, 0x01,     //Report Size (1)
0x95, (HID_REPORT_BYTES - 1) * 8, //  Report Count (104)
0x81, 0x02,     //      Input (Data, Variable, Absolute) 
// Wrapping up - 1 byte
0xC0			//  End Collection
// Total bytes = 78
```

Therefore the resulting `c` structure would be

``` c
struct nkroHID_t {
    uint8_t modifiers;
    uint8_t padding[7];
    uint8_t bitmap[HID_REPORT_BYTES - 1];
};
```

# Appendix

## Boot keyboard descriptor - Documentation

``` c
// Keyboard - 8 bytes
0x05, 0x01,     //  USAGE_PAGE(generic desktop)
0x09, 0x06,     //  USAGE(keyboard)
0xA1, 0x01,     //  COLLECTION(application)
0x05, 0x07,     //      USAGE_PAGE(key codes)
// modifier byte - 14 bytes
0x19, 0xE0,     //      USAGE_MINIMUM(left control)
0x29, 0xE7,     //      USAGE_MAXIMUM(right gui)
0x15, 0x00,     //      LOGICAL_MINIMUM(0)
0x25, 0x01,     //      LOGICAL_MAXIMUM(1)
0x75, 0x01,     //      REPORT_SIZE(1)
0x95, 0x08,     //      REPORT_COUNT(8)
0x81, 0x02,     //      INPUT(data, var, abs)
// reserved byte - 6 bytes
0x95, 0x01,     //      REPORT_COUNT(1)
0x75, 0x08,     //      REPORT_SIZE(8)
0x81, 0x01,     //      INPUT(constant)
// LEDs - 12 bytes
0x95, 0x05,     //      REPORT_COUNT(5)
0x75, 0x01,     //      REPORT_SIZE(1)
0x05, 0x08,     //      USAGE_PAGE(LEDs)
0x19, 0x01,     //      USAGE_MINIMUM(num lock)
0x29, 0x05,     //      USAGE_MAXIMUM(Kana)
0x91, 0x02,     //      OUTPUT(data, var, abs)
// LED bit padding - 6 bytes
0x95, 0x01,     //      REPORT_COUNT(1)
0x75, 0x03,     //      REPORT_SIZE(3)
0x91, 0x01,     //      OUTPUT(const, var, abs) 
// 6KRO padding - 16 bytes
0x95, 0x06,     //      REPORT_COUNT(6)
0x75, 0x08,     //      REPORT_SIZE(8)
0x15, 0x00,     //      LOGICAL_MINIMUM(0)
0x25, 0x65,     //      LOGICAL_MAXIMUM(101)
0x05, 0x07,     //      USAGE_PAGE(key codes)
0x19, 0x01,     //      USAGE_MINIMUM(0)
0x29, 0x05,     //      USAGE_MAXIMUM(101)
0x81, 0x00,     //      INPUT(const, array, abs)
// Wrapping up - 1 byte
0xC0,           //  END_COLLECTION
// Total bytes = 63
```

``` c
ruct bootHID_t {                                                               
  uint8_t modifiers;                                                           
  uint8_t reserverd;                                                           
  uint8_t keys[6];                                                             
}; 
```

## Boot keyboard descriptor - Tested working

``` c
// Keyboard 63 bytes
0x05, 0x01,     //  USAGE_PAGE(generic desktop)
0x09, 0x06,     //  USAGE(keyboard)
0xA1, 0x01,     //  COLLECTION(application)
0x05, 0x07,     //      USAGE_PAGE(keyboard)
// modifiers
0x75, 0x01,     //      REPORT_SIZE(1)
0x95, 0x08,     //      REPORT_COUNT(8)
0x19, 0xE0,     //      USAGE_MINIMUM(left control)
0x29, 0xE7,     //      USAGE_MAXIMUM(right gui)
0x15, 0x00,     //      LOGICAL_MINIMUM(0)
0x25, 0x01,     //      LOGICAL_MAXIMUM(1)
0x81, 0x02,     //      INPUT(data, var, abs)
//reserved byte
0x95, 0x01,     //      REPORT_COUNT(1)
0x75, 0x08,     //      REPORT_SIZE(8)
0x81, 0x01,     //      INPUT(constant)
// Keycodes
0x05, 0x07,     //      USAGE_PAGE(keyboard)
0x95, 0x06,     //      REPORT_COUNT(6)
0x75, 0x08,     //      REPORT_SIZE(8)
0x15, 0x00,     //      LOGICAL_MINIMUM(0)
0x25, 0x65,     //      LOGICAL_MAXIMUM(101)
0x19, 0x00,     //      USAGE_MINIMUM(0)
0x29, 0x65,     //      USAGE_MAXIMUM(101)
0x81, 0x00,     //      INPUT(data, array, abs)
// LEDs
0x05, 0x08,     //      USAGE_PAGE(LEDs)
0x75, 0x01,     //      REPORT_SIZE(1)
0x95, 0x05,     //      REPORT_COUNT(5)
0x19, 0x01,     //      USAGE_MINIMUM(num lock)
0x29, 0x05,     //      USAGE_MAXIMUM(Kana)
0x91, 0x02,     //      OUTPUT(data, var, abs)
// LED bit padding - 6 bytes
0x75, 0x03,     //      REPORT_SIZE(3)
0x95, 0x01,     //      REPORT_COUNT(1)
0x91, 0x01,     //      OUTPUT(const, var, abs)  **padding**
0xC0,           //  END_COLLECTION
```

## LEDs

``` c
// LEDs - 12 bytes
0x95, 0x05,     //      REPORT_COUNT(5)
0x75, 0x01,     //      REPORT_SIZE(1)
0x05, 0x08,     //      USAGE_PAGE(LEDs)
0x19, 0x01,     //      USAGE_MINIMUM(num lock)
0x29, 0x05,     //      USAGE_MAXIMUM(Kana)
0x91, 0x02,     //      OUTPUT(data, var, abs)
// LED bit padding - 6 bytes
0x95, 0x01,     //      REPORT_COUNT(1)
0x75, 0x03,     //      REPORT_SIZE(3)
0x91, 0x01,     //      OUTPUT(const, var, abs) 
```
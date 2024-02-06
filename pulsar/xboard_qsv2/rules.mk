# MCU name (Actually F070, but close enough)
MCU = STM32F072

# This file intentionally left blank
BOARD = PULSAR_STM32_F070

# Bootloader selection
BOOTLOADER = stm32-dfu

# Build Options (change yes to no to disable)
BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes       # Mouse keys(+4700)
EXTRAKEY_ENABLE = yes       # Audio control and System control(+450)
CONSOLE_ENABLE = no         # Console for debug(+400)
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = yes           # Enable N-Key Rollover
BACKLIGHT_ENABLE = no       		# Enable keyboard backlight functionality
LED_MATRIX_ENABLE = no				#
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow (WS2811, WS2812, WS2812B, WS2812C, SK6812, SK6812MINI, SK6805, APA102)
RGB_MATRIX_ENABLE = yes			#
RGB_MATRIX_DRIVER = ws2812		# IS31FL3731, 3733, 3737, WS2812등등 (WS2811, WS2812, WS2812B, WS2812C, SK6812, SK6812MINI, SK6805, APA102)
WS2812_DRIVER = spi				# bitbang, spi(circular buffer issue), pwm(too much access, Noise도 심한듯)

AUDIO_ENABLE = no           # Audio output
LTO_ENABLE = yes

DYNAMIC_MACRO_ENABLE = yes
WPM_ENABLE = yes

ENCODER_ENABLE = yes        # Enable Encoder
ENCODER_MAP_ENABLE = yes

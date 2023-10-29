# see https://docs.qmk.fm/#/config_options?id=feature-options
CAPS_WORD_ENABLE = yes
COMBO_ENABLE = yes
DEFERRED_EXEC_ENABLE = yes
MOUSEKEY_ENABLE = yes
RGB_MATRIX_CUSTOM_USER = yes
TAP_DANCE_ENABLE = yes
VIA_ENABLE = no

# "yes" or "no"
IS_PRODUCTION ?= yes

ifeq ($(strip $(IS_PRODUCTION)), yes)
	LTO_ENABLE = yes
	CONSOLE_ENABLE = no
else
	LTO_ENABLE = no
	CONSOLE_ENABLE = yes
endif
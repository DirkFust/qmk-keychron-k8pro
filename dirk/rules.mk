# see https://docs.qmk.fm/#/config_options?id=feature-options
VIA_ENABLE = no
MOUSEKEY_ENABLE = yes
CAPS_WORD_ENABLE = yes
DEFERRED_EXEC_ENABLE = yes
RGB_MATRIX_CUSTOM_USER = yes
TAP_DANCE_ENABLE = yes

# "yes" or "no"
IS_PRODUCTION ?= yes

ifeq ($(strip $(IS_PRODUCTION)), yes)
	LTO_ENABLE = yes
	CONSOLE_ENABLE = no
else
	LTO_ENABLE = no
	CONSOLE_ENABLE = yes
endif
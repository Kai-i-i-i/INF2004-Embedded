add_library(menu MENU.h MENU.c)

# pull in common dependencies
target_link_libraries(menu pico_stdlib)

# create map/bin/hex file etc.
#pico_add_extra_outputs(spifunc)

# add url via pico_set_program_url
#example_auto_set_url(forensic)

#pico_enable_stdio_usb(forensic 1)
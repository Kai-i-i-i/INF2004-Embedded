add_library(conversion_functions conversion.h conversion.c)

# pull in common dependencies
target_link_libraries(conversion_functions pico_stdlib)

# create map/bin/hex file etc.
#pico_add_extra_outputs(spifunc)

# add url via pico_set_program_url
#example_auto_set_url(forensic)

#pico_enable_stdio_usb(forensic 1)
add_library(spi_functions SPIFUNCS.h SPIFUNCS.c)

# pull in common dependencies
target_link_libraries(spi_functions pico_stdlib hardware_spi)

# create map/bin/hex file etc.
#pico_add_extra_outputs(spifunc)

# add url via pico_set_program_url
#example_auto_set_url(forensic)

#pico_enable_stdio_usb(forensic 1)
add_executable(inj_rec
        inj_rec.c
        )

add_subdirectory(conversion)
add_subdirectory(SPIFUNCS)
add_subdirectory(menu)

# pull in common dependencies
#target_link_libraries(inj_rec pico_stdlib hardware_gpio pico_cyw43_arch_lwip_threadsafe_background ico_lwip_iperf)
target_link_libraries(inj_rec pico_stdlib hardware_gpio spi_stuff conversions pico_stdio pico_stdio_uart menu)

# enable usb output and uart output
# modify here as required
pico_enable_stdio_usb(inj_rec 1)
pico_enable_stdio_uart(inj_rec 1)

# Add the pico_stdio library
#target_link_libraries(inj_rec pico_stdio)

# Add the pico_stdio_uart library
#target_link_libraries(inj_rec pico_stdio_uart)

# create map/bin/hex file etc.
pico_add_extra_outputs(inj_rec)

# add url via pico_set_program_url
example_auto_set_url(inj_rec)


add_executable(i2c_proj i2c_proj.c)

# pull in common dependencies
target_link_libraries(i2c_proj pico_stdlib hardware_i2c)

# enable usb output, enable uart output
pico_enable_stdio_usb(i2c_proj 1)
pico_enable_stdio_uart(i2c_proj 1)

# create map/bin/hex file etc
pico_add_extra_outputs(i2c_proj)

# add url via pico_set_program_url
example_auto_set_url(i2c_proj)
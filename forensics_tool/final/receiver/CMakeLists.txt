add_executable(receiver
        receiver.c
        )
# pull in common dependencies
target_link_libraries(receiver pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(receiver)

# add url via pico_set_program_url
example_auto_set_url(receiver)

pico_enable_stdio_usb(receiver 1)
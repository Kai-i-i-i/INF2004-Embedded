add_executable(sender
        sender.c
        )
# pull in common dependencies
target_link_libraries(sender pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(sender)

# add url via pico_set_program_url
example_auto_set_url(sender)

pico_enable_stdio_usb(sender 1)
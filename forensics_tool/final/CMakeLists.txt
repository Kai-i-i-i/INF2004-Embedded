add_subdirectory(spireceiver)
add_subdirectory(uartsender)
add_subdirectory(uartreceiver)

add_executable(selectModule
        selectModule.c
        )
# pull in common dependencies
target_link_libraries(selectModule pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(selectModule)

# add url via pico_set_program_url
example_auto_set_url(selectModule)

pico_enable_stdio_usb(selectModule 1)
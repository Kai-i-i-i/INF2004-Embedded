add_executable(uart_test_rx
        uart_test_rx.c
        )
        add_executable(uart_test_tx
        uart_test_tx.c
        )

# pull in common dependencies
#target_link_libraries(inj_rec pico_stdlib hardware_gpio pico_cyw43_arch_lwip_threadsafe_background ico_lwip_iperf)
target_link_libraries(uart_test_rx pico_stdlib hardware_gpio pico_stdio pico_stdio_uart)
target_link_libraries(uart_test_tx pico_stdlib hardware_gpio pico_stdio pico_stdio_uart)

# enable usb output and uart output
# modify here as required
pico_enable_stdio_usb(uart_test_rx 1)
pico_enable_stdio_uart(uart_test_rx 1)

pico_enable_stdio_usb(uart_test_tx 1)
pico_enable_stdio_uart(uart_test_tx 1)

# Add the pico_stdio library
#target_link_libraries(inj_rec pico_stdio)

# Add the pico_stdio_uart library
#target_link_libraries(inj_rec pico_stdio_uart)

# create map/bin/hex file etc.
pico_add_extra_outputs(uart_test_rx)
pico_add_extra_outputs(uart_test_tx)

# add url via pico_set_program_url
example_auto_set_url(uart_test_rx)
example_auto_set_url(uart_test_tx)


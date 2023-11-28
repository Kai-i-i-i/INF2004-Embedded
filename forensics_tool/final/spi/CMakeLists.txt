add_executable(spi 
        spiReceive.c 
        ) 
 
# pull in common dependencies 
target_link_libraries(spi pico_stdlib) 
target_link_libraries(spi pico_stdlib hardware_spi) 
target_link_libraries(spi pico_stdlib hardware_uart) 
target_link_libraries(spi pico_stdlib pico_cyw43_arch_none) 
 
pico_enable_stdio_usb(spi 1) 
pico_enable_stdio_uart(spi 1) 
 
# create map/bin/hex file etc. 
pico_add_extra_outputs(spi) 
 
# add url via pico_set_program_url 
example_auto_set_url(spi)
add_executable(inj_rec
        inj_rec.c
        )

# pull in common dependencies
#target_link_libraries(inj_rec pico_stdlib hardware_gpio pico_cyw43_arch_lwip_threadsafe_background ico_lwip_iperf)
target_link_libraries(inj_rec pico_stdlib hardware_gpio)

# target_include_directories(inj_rec PRIVATE
# ${CMAKE_CURRENT_LIST_DIR}
# )

# include the "pico/cyw43.h" library
# target_sources(inj_rec PRIVATE
#         ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_arch/cyw43_arch.c
#         ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_driver/cyw43_driver.c
#         ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_driver/lwip_nosys.c
#         )

# target_include_directories(inj_rec PRIVATE
#         ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_arch/include
#         ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_driver/include
#         ${PICO_SDK_PATH}/src/rp2_common/pico_lwip/include
#         )

# enable usb output and uart output
# modify here as required
pico_enable_stdio_usb(inj_rec 1)
pico_enable_stdio_uart(inj_rec 1)

# Add the pico_stdio library
target_link_libraries(inj_rec pico_stdio)

# Add the pico_stdio_uart library
target_link_libraries(inj_rec pico_stdio_uart)

# create map/bin/hex file etc.
pico_add_extra_outputs(inj_rec)

# add url via pico_set_program_url
example_auto_set_url(inj_rec)

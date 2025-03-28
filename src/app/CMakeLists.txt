set(APP_NAME "app")

add_executable(${APP_NAME}
    app_main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sht45/src/driver_sht4x.c
)

target_link_libraries(${APP_NAME} PRIVATE
    ${RodeoIot_LIBRARIES}
)

target_include_directories(${APP_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/sht45/src
)

add_custom_command(
    TARGET ${APP_NAME}
    POST_BUILD
        COMMAND ${CMAKE_OBJCOPY}
        ARGS -O binary ${APP_NAME}.axf ${APP_NAME}.bin
 )

set_target_properties(${APP_NAME} PROPERTIES OUTPUT_NAME "${APP_NAME}.axf")

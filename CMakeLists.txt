# Crear un ejecutable, no un "utility target"
add_executable(${APP_NAME} ${OBJECT_FILES})

# Enlazar las bibliotecas necesarias
target_link_libraries(${APP_NAME} ${CMAKE_THREAD_LIBS_INIT} -lbcm2835)

# Si hay otras bibliotecas específicas según la arquitectura, agrégalas
if(ARCH STREQUAL "x86_64")
    # Aquí puedes agregar más bibliotecas si es necesario para x86_64
    # target_link_libraries(${APP_NAME} -lSSD1306_OLED_RPI)
    message(STATUS "x86_64 detectado")
elseif(ARCH STREQUAL "aarch64")
    # Aquí puedes agregar más bibliotecas si es necesario para aarch64
    # target_link_libraries(${APP_NAME} -lSSD1306_OLED_RPI)
    message(STATUS "aarch64 detectado")
elseif(ARCH STREQUAL "armv7l")
    # No es necesario agregar ninguna biblioteca para armv7l, según el Makefile original
    message(STATUS "armv7l detectado")
endif()

# Agregar las reglas de limpieza
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${OBJ_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove ${APP_PATH}
)

# Regla para correr el ejecutable (si es necesario usar sudo)
add_custom_target(run
    COMMAND sudo ${APP_PATH}
)

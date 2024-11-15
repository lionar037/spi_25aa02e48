# Definir el nombre del proyecto y la ruta del ejecutable
set(PROJECT_NAME "st25vf010_app")
set(APP_NAME ${PROJECT_NAME}_${ARCH})
set(APP_PATH "${CMAKE_BINARY_DIR}/bin/${APP_NAME}")
set(OBJ_DIR "${CMAKE_BINARY_DIR}/obj")

# Definir las rutas de los archivos objeto
set(SOURCE_FILES 
    # Aquí van los archivos fuente, por ejemplo:
    src/main.cpp
    src/other_file.cpp
)
set(OBJECT_FILES ${SOURCE_FILES})  # Deberías incluir los objetos generados a partir de los archivos fuente

# Crear un ejecutable, no un "utility target"
add_executable(${APP_NAME} ${OBJECT_FILES})

# Incluir los directorios de cabeceras (si es necesario)
include_directories(${PROJECT_SOURCE_DIR}/include)

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
    # Aquí puedes agregar bibliotecas para armv7l si es necesario
    # No es necesario agregar ninguna biblioteca para armv7l según el Makefile original
    message(STATUS "armv7l detectado")
else()
    message(WARNING "Arquitectura desconocida: ${ARCH}")
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

# Limpiar el proyecto (sin borrar el ejecutable final)
add_custom_target(clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${OBJ_DIR}
)

# Regla para mostrar las variables
add_custom_target(print-vars
    COMMAND ${CMAKE_COMMAND} -E echo "INCDIRS = ${INCDIRS}"
    COMMAND ${CMAKE_COMMAND} -E echo "CFLAGS = ${CFLAGS}"
    COMMAND ${CMAKE_COMMAND} -E echo "ARCH = ${ARCH}"
)

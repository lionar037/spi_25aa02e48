cmake_minimum_required(VERSION 3.25)
project(st25vf010_project)

# Definir la arquitectura (si no se pasa como argumento de CMake)
set(ARCH "armv7l")  # O la arquitectura adecuada

# Definir el nombre del proyecto y la ruta del ejecutable
set(PROJECT_NAME "st25vf010_app")
set(APP_NAME ${PROJECT_NAME}_${ARCH})
set(APP_PATH "${CMAKE_BINARY_DIR}/bin/${APP_NAME}")
set(OBJ_DIR "${CMAKE_BINARY_DIR}/obj")

# Definir las rutas de los archivos objeto
set(SOURCE_FILES 
    src/main.cpp
    # src/other_file.cpp  # Eliminar si no existe
)
set(OBJECT_FILES ${SOURCE_FILES})

# Crear un ejecutable, no un "utility target"
add_executable(${APP_NAME} ${OBJECT_FILES})

# Incluir los directorios de cabeceras (si es necesario)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Enlazar las bibliotecas necesarias
target_link_libraries(${APP_NAME} ${CMAKE_THREAD_LIBS_INIT} -lbcm2835)

# Si hay otras bibliotecas específicas según la arquitectura, agrégalas
if(ARCH STREQUAL "x86_64")
    message(STATUS "x86_64 detectado")
elseif(ARCH STREQUAL "aarch64")
    message(STATUS "aarch64 detectado")
elseif(ARCH STREQUAL "armv7l")
    message(STATUS "armv7l detectado")
else()
    message(WARNING "Arquitectura desconocida: ${ARCH}")
endif()

# Agregar las reglas de limpieza
add_custom_target(clean-all-target
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
    COMMAND ${CMAKE_COMMAND} -E echo "ARCH = ${ARCH}"
)

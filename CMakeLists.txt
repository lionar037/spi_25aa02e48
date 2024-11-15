# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(st25vf010_app)

# Detectar arquitectura (32 o 64 bits)
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    set(ARCH "x86_64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    set(ARCH "aarch64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "armv7l")
    set(ARCH "armv7l")
else()
    set(ARCH "unknown")
endif()

# Definir las rutas de las bibliotecas y los directorios de encabezados
set(PROJECT_NAME src)
set(OBJ_DIR obj)
set(LIB_DIR include)

# Crear el directorio de salida para la aplicación
set(APP_DIR bin)
set(APP_NAME st25vf010_app_${ARCH})
set(APP_PATH ${APP_DIR}/${APP_NAME})

# Definir las opciones de compilación
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -MMD -MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -MMD -MP")

# Detectar si estamos en modo de depuración
if(DEFINED ENV{DBG})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

# Añadir las bibliotecas necesarias
find_package(Threads REQUIRED)

# Configurar bibliotecas adicionales según la arquitectura
if(ARCH STREQUAL "x86_64")
    message(STATUS "x86_64 detectado")
elseif(ARCH STREQUAL "aarch64")
    message(STATUS "aarch64 detectado")
elseif(ARCH STREQUAL "armv7l")
    message(STATUS "armv7l detectado")
endif()

# Incluir directorios
include_directories(${PROJECT_NAME} ${LIB_DIR})

# Incluir todos los archivos fuente .cpp y .c
file(GLOB_RECURSE ALL_CPP_FILES ${PROJECT_NAME}/*.cpp)
file(GLOB_RECURSE ALL_C_FILES ${PROJECT_NAME}/*.c)

# Definir los objetos
set(SOURCE_FILES ${ALL_CPP_FILES} ${ALL_C_FILES})
set(OBJECT_FILES ${SOURCE_FILES})
set(DEPENDENCY_FILES)

# Crear los archivos objeto
foreach(SOURCE_FILE ${SOURCE_FILES})
    get_filename_component(SOURCE_NAME ${SOURCE_FILE} NAME_WE)
    set(OBJ_FILE ${OBJ_DIR}/${SOURCE_NAME}.o)
    list(APPEND OBJECT_FILES ${OBJ_FILE})
    add_custom_command(
        OUTPUT ${OBJ_FILE}
        COMMAND ${CMAKE_CXX_COMPILER} -c ${SOURCE_FILE} -o ${OBJ_FILE} ${CMAKE_CXX_FLAGS} -I${PROJECT_NAME} -I${LIB_DIR}
        DEPENDS ${SOURCE_FILE}
    )
endforeach()

# Enlazar los archivos objeto y crear la aplicación
add_custom_target(${APP_NAME} ALL DEPENDS ${OBJECT_FILES})

# Enlazar bibliotecas y dependencias (como pthread y bcm2835)
target_link_libraries(${APP_NAME} ${CMAKE_THREAD_LIBS_INIT} -lbcm2835)

# Regla de limpieza
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${OBJ_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove ${APP_PATH}
)

# Regla para correr el ejecutable (si es necesario usar sudo)
add_custom_target(run
    COMMAND sudo ${APP_PATH}
)

# Mostrar las variables
add_custom_target(print-vars
    COMMAND ${CMAKE_COMMAND} -E echo "INCDIRS = ${PROJECT_NAME} ${LIB_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "CFLAGS = ${CMAKE_CXX_FLAGS}"
)

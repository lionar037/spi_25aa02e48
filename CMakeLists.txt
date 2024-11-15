cmake_minimum_required(VERSION 3.5)

project(sst25vf010a)

set(CMAKE_CXX_STANDARD 17)

#find_library(BCM2835_LIB bcm2835)

# Primer ejecutable: app
add_executable(read_sst25vf read_sst25vf.cc spi.cc)

target_link_libraries(read_sst25vf)

# Segundo ejecutable: app2
add_executable(write_sst25vf write_sst25vf.cc spi.cc)
target_link_libraries(write_sst25vf)


# Segundo ejecutable: app3
add_executable(erase_sst25vf erase_sst25vf.cc spi.cc)
target_link_libraries(erase_sst25vf)

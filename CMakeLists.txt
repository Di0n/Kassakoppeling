cmake_minimum_required(VERSION 2.6)
project(kassakoppeling)

add_executable(kassakoppeling webclient.cpp utils.cpp task.cpp datetime.cpp logger.cpp http.cpp supermarket.cpp active_product.cpp product.cpp errorhandler.cpp config_file.cpp supermarket_base.cpp utility.cpp rs232.c serial_reader.cpp main.cpp)

install(TARGETS kassakoppeling RUNTIME DESTINATION bin)

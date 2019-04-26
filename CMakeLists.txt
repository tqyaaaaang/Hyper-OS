CMAKE_MINIMUM_REQUIRED ( VERSION 3.1 )

SET ( CMAKE_CXX_STANDARD 11 )

PROJECT ( HYPER-OS )

SET ( src_list
    src/main.cpp
    src/trap/trap.cpp
)

SET ( prog_name hos )

SET ( CMAKE_INSTALL_PREFIX )

SET ( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
ADD_EXECUTABLE ( ${prog_name} ${src_list} )

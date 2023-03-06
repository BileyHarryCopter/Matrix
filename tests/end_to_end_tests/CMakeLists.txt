add_executable (random_matrix 
                src/random_matrix_driver.cpp)

add_executable (integral_matrix
                src/integral_matrix_driver.cpp)

add_executable (real_matrix
                src/real_matrix_driver.cpp)

target_link_directories(random_matrix
                        PRIVATE ../../include)

target_link_directories(integral_matrix
                        PRIVATE ../../include)

target_link_directories(real_matrix
                        PRIVATE ../../include)
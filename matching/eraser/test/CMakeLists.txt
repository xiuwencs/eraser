add_executable (eraser eraser.cpp)
target_link_libraries(eraser ${PROJECT_NAME})

add_executable (eraser-ac eraser-AC.cpp)
target_link_libraries(eraser-ac ${PROJECT_NAME})

add_executable (naive naive.cpp)
target_link_libraries(naive ${PROJECT_NAME})

add_executable (naive-ac naive-AC.cpp)
target_link_libraries(naive-ac ${PROJECT_NAME})

add_executable (sdch-ac sdch-AC.cpp)
target_link_libraries(sdch-ac ${PROJECT_NAME})

add_executable (twins twins.cpp)
target_link_libraries(twins ${PROJECT_NAME})

add_executable (testDFA testDFA.cpp)
target_link_libraries(testDFA ${PROJECT_NAME})
set(SOURCES main.cpp 
            servo.cpp 
            trottle.cpp 
            joystick.cpp)
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS . ../include)
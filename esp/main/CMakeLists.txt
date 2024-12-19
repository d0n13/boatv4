set(SOURCES main.cpp 
            servo.cpp 
            trottle.cpp 
            joystick.cpp
            # display/ui.c
            display/display.c
            map.cpp)
            
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS .)
cmake_minimum_required(VERSION 3.0)

set(MY_ADDITIONAL_SOURCES
    main/a_competition.cpp
    rt/motionblur.h
)

set(ASSIGNMENT_SOURCES
    rt/primmod/bmap.cpp
    rt/primmod/bmap.h
    ${MY_ADDITIONAL_SOURCES}
    main/a_bump.cpp
)

file(COPY
    models/stones.png
    models/n-final.png
    models/pyramid_front.obj
    models/pyramid_back.obj
    models/statues.obj
    DESTINATION ${CMAKE_BINARY_DIR}/models
)

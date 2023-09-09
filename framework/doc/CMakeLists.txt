if(NOT ${PLANTUML} MATCHES "PLANTUML-NOTFOUND")
    add_custom_target(class-diagram-events ALL
        SOURCES class-diagram-events
        COMMAND ${PLANTUML} -tsvg ${CMAKE_CURRENT_SOURCE_DIR}/class-diagram-events -o ${CMAKE_CURRENT_SOURCE_DIR}
        )
    add_custom_target(class-diagram-event-systems ALL
        SOURCES class-diagram-event-systems
        COMMAND ${PLANTUML} -tsvg ${CMAKE_CURRENT_SOURCE_DIR}/class-diagram-event-systems -o ${CMAKE_CURRENT_SOURCE_DIR}
        )
endif()

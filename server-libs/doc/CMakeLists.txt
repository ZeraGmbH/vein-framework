if(NOT ${PLANTUML} MATCHES "PLANTUML-NOTFOUND")
    add_custom_target(class-diagram-server-event-systems ALL
        SOURCES class-diagram-server-event-systems
        COMMAND ${PLANTUML} -tsvg ${CMAKE_CURRENT_SOURCE_DIR}/class-diagram-server-event-systems -o ${CMAKE_CURRENT_SOURCE_DIR}
        )
endif()

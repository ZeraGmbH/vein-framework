if(NOT ${PLANTUML} MATCHES "PLANTUML-NOTFOUND")
    add_custom_target(class-diagram-client-event-systems ALL
        SOURCES class-diagram-client-event-systems
        COMMAND ${PLANTUML} -tsvg ${CMAKE_CURRENT_SOURCE_DIR}/class-diagram-client-event-systems -o ${CMAKE_CURRENT_SOURCE_DIR}
        )
endif()

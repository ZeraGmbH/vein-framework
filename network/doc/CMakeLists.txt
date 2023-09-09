if(NOT ${PLANTUML} MATCHES "PLANTUML-NOTFOUND")
    add_custom_target(class-diagram-network-event-systems ALL
        SOURCES class-diagram-network-event-systems
        COMMAND ${PLANTUML} -tsvg ${CMAKE_CURRENT_SOURCE_DIR}/class-diagram-network-event-systems -o ${CMAKE_CURRENT_SOURCE_DIR}
        )
endif()

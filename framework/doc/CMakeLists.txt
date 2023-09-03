if(NOT ${PLANTUML} MATCHES "PLANTUML-NOTFOUND")
    add_custom_target(framework-doc ALL
        SOURCES class-diagram-events
        COMMAND ${PLANTUML} -tsvg ${CMAKE_CURRENT_SOURCE_DIR}/class-diagram-events -o ${CMAKE_CURRENT_SOURCE_DIR}
        )
endif()

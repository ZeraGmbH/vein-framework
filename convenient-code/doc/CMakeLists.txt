if(NOT ${PLANTUML} MATCHES "PLANTUML-NOTFOUND")
    add_custom_target(convenient-code-doc ALL
        SOURCES class-diagram
        COMMAND ${PLANTUML} -tsvg ${CMAKE_CURRENT_SOURCE_DIR}/class-diagram -o ${CMAKE_CURRENT_SOURCE_DIR}
        )
endif()

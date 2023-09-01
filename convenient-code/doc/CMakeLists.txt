if(NOT ${PLANTUML} MATCHES "PLANTUML-NOTFOUND")
    add_custom_target(convenient-code-doc ALL
        SOURCES convenient-code-uml
        COMMAND ${PLANTUML} -tsvg ${CMAKE_CURRENT_SOURCE_DIR}/convenient-code-uml -o ${CMAKE_CURRENT_SOURCE_DIR}
        )
endif()

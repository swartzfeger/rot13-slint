if (NOT DEFINED APP_EXECUTABLE OR NOT DEFINED APP_DIR)
    message(FATAL_ERROR "BundleWindows.cmake requires APP_EXECUTABLE and APP_DIR.")
endif()

if (NOT DEFINED RUNTIME_SEARCH_DIRS)
    set(RUNTIME_SEARCH_DIRS "")
endif()

file(GET_RUNTIME_DEPENDENCIES
    EXECUTABLES "${APP_EXECUTABLE}"
    DIRECTORIES ${RUNTIME_SEARCH_DIRS}
    RESOLVED_DEPENDENCIES_VAR resolved_dependencies
    UNRESOLVED_DEPENDENCIES_VAR unresolved_dependencies
    PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
    POST_EXCLUDE_REGEXES "[Ww][Ii][Nn][Dd][Oo][Ww][Ss][/\\\\].*" "[Ss]ystem32[/\\\\].*"
)

foreach (dependency IN LISTS resolved_dependencies)
    get_filename_component(dependency_name "${dependency}" NAME)
    file(COPY_FILE "${dependency}" "${APP_DIR}/${dependency_name}" ONLY_IF_DIFFERENT)
endforeach()

if (unresolved_dependencies)
    message(WARNING "Unresolved runtime dependencies: ${unresolved_dependencies}")
endif()

if (NOT DEFINED APP_EXECUTABLE OR NOT DEFINED BUNDLE_FRAMEWORKS_DIR OR NOT DEFINED SLINT_DYLIB OR NOT DEFINED SLINT_DYLIB_NAME)
    message(FATAL_ERROR "BundleMacOS.cmake requires APP_EXECUTABLE, BUNDLE_FRAMEWORKS_DIR, SLINT_DYLIB, and SLINT_DYLIB_NAME.")
endif()

set(bundled_slint_dylib "${BUNDLE_FRAMEWORKS_DIR}/${SLINT_DYLIB_NAME}")
set(bundled_slint_id "@executable_path/../Frameworks/${SLINT_DYLIB_NAME}")

file(MAKE_DIRECTORY "${BUNDLE_FRAMEWORKS_DIR}")
file(COPY_FILE "${SLINT_DYLIB}" "${bundled_slint_dylib}" ONLY_IF_DIFFERENT)

execute_process(
    COMMAND otool -D "${SLINT_DYLIB}"
    OUTPUT_VARIABLE slint_dylib_id_output
    ERROR_VARIABLE slint_dylib_id_error
    RESULT_VARIABLE slint_dylib_id_result
)

if (NOT slint_dylib_id_result EQUAL 0)
    message(FATAL_ERROR "Could not inspect Slint dylib install name: ${slint_dylib_id_error}")
endif()

string(REPLACE "\n" ";" slint_dylib_id_lines "${slint_dylib_id_output}")
list(GET slint_dylib_id_lines 1 slint_dylib_id)

execute_process(
    COMMAND install_name_tool -id "${bundled_slint_id}" "${bundled_slint_dylib}"
    RESULT_VARIABLE install_name_id_result
)

if (NOT install_name_id_result EQUAL 0)
    message(FATAL_ERROR "Could not update bundled Slint dylib install name.")
endif()

execute_process(
    COMMAND install_name_tool -change "${slint_dylib_id}" "${bundled_slint_id}" "${APP_EXECUTABLE}"
    RESULT_VARIABLE install_name_change_result
)

if (NOT install_name_change_result EQUAL 0)
    message(FATAL_ERROR "Could not update app executable Slint dylib reference.")
endif()

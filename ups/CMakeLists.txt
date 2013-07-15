# cmake driver file for @ups/@

execute_process( COMMAND date
                 OUTPUT_VARIABLE datime
                 OUTPUT_STRIP_TRAILING_WHITESPACE )

# create package configuration and version files

process_ups_files()

configure_file ( ${gm2geom_SOURCE_DIR}/ups/gm2geom-config.cmake.in
                 ${gm2geom_BINARY_DIR}/ups/gm2geom-config.cmake @ONLY )
configure_file ( ${gm2geom_SOURCE_DIR}/ups/gm2geom-config-version.cmake.in
                 ${gm2geom_BINARY_DIR}/ups/gm2geom-config-version.cmake @ONLY )

install (FILES ${gm2geom_BINARY_DIR}/ups/gm2geom-config.cmake
               ${gm2geom_BINARY_DIR}/ups/gm2geom-config-version.cmake
	 DESTINATION ${flavorqual_dir}/lib/gm2geom-${version} )

# Add setup line to source_for_local.sh in local products area (no qualifier needed here)
install(CODE "file(APPEND ${CMAKE_INSTALL_PREFIX}/source_for_local.sh \"\nsetup -j ${product} ${version} -q \\\"${full_qualifier}\\\"\")")


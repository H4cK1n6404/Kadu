#cmakedefine DEBUG_ENABLED 1

#define KADU_DATADIR "${KADU_DATADIR}"
#define KADU_PLUGINS_LIBDIR "${KADU_PLUGINS_LIBDIR}"
#define KADU_DESKTOP_FILE_PATH "${KADU_DESKTOP_FILE_DIR}/${KADU_DESKTOP_FILE_NAME}"

#define VERSION "${KADU_VERSION}"
#define BUILD_DESCRIPTION "${BUILD_DESCRIPTION}"
#define NUMERIC_VERSION ${NUMERIC_VERSION}

#cmakedefine01 SIG_HANDLING_ENABLED
#cmakedefine01 HAVE_EXECINFO
#cmakedefine01 WITH_LIBINDICATE_QT

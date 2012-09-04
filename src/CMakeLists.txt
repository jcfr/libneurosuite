project(libklustersshared-src)
set(libklustersshared_SRCS 
	qrecentfileaction.cpp 
	qcolorbutton.cpp 
	qpagedialog.cpp
	qpagemodel.cpp
	qpageview.cpp
	qpageview_p.cpp
	qpagewidget.cpp
	qpagewidgetmodel.cpp
	qextenddialog.cpp
	kseparator.cpp
	)
set (libklustersshared_HEADERS 
	libklustersshared_export.h 
	qrecentfileaction.h 
	qcolorbutton.h 
	qpagedialog.h 
	)


add_library (klustersshared SHARED ${libklustersshared_SRCS})
target_link_libraries( klustersshared ${QT_LIBRARIES})

install( TARGETS klustersshared EXPORT LibKlustersSharedLibraryTargets 
    RUNTIME DESTINATION ${BIN_INSTALL_DIR} 
    LIBRARY DESTINATION ${LIB_INSTALL_DIR} 
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR} 
)

install( FILES ${libklustersshared_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR} COMPONENT Devel )


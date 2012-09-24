project(libklustersshared-src)
set(libklustersshared_SRCS 
	gui/qrecentfileaction.cpp 
	gui/qcolorbutton.cpp 
	gui/qpagedialog.cpp
	gui/qpagemodel.cpp
	gui/qpageview.cpp
	gui/qpageview_p.cpp
	gui/qpagewidget.cpp
	gui/qpagewidgetmodel.cpp
	gui/qextenddialog.cpp
	gui/kseparator.cpp
        gui/dockarea.cpp
	gui/qextendtabwidget.cpp
	shared/channelcolors.cpp
	shared/itemcolors.cpp
	shared/zoomwindow.cpp
	)

set (libklustersshared_HEADERS 
	libklustersshared_export.h 
	gui/qrecentfileaction.h 
	gui/qcolorbutton.h 
	gui/qpagedialog.h
       gui/qextenddialog.h
       gui/qpagewidget.h
       gui/qpagemodel.h
       gui/qpageview.h
       gui/qpagewidgetmodel.h
       gui/dockarea.h
       gui/qextendtabwidget.h
       shared/channelcolors.h
       shared/itemcolors.h
       shared/types.h
       shared/array.h
       shared/zoomwindow.h
       )


SET(translate_files
    klustershared.ts
)

if(UPDATE_TRANSLATION)
QT4_CREATE_TRANSLATION(translateSrcs ../translate ${libklustersshared_SRCS} ${translate_files})
endif()

qt4_add_resources(libklustersshared_SRCS gui/libklustersshared-icons.qrc)

add_library (klustersshared SHARED ${libklustersshared_SRCS} ${translateSrcs})
target_link_libraries( klustersshared ${QT_LIBRARIES})

install( TARGETS klustersshared EXPORT LibKlustersSharedLibraryTargets 
    RUNTIME DESTINATION ${BIN_INSTALL_DIR} 
    LIBRARY DESTINATION ${LIB_INSTALL_DIR} 
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR} 
)

install( FILES ${libklustersshared_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR} COMPONENT Devel )


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
	gui/qhelpviewer.cpp
	shared/channelcolors.cpp
	shared/itemcolors.cpp
	shared/zoomwindow.cpp
	shared/utilities.cpp
	#remove it in qt5
	libinqt5/qstandardpaths.cpp
	)

#remove it in qt5
if (UNIX)
  set(libklustersshared_SRCS ${libklustersshared_SRCS} libinqt5/qstandardpaths_unix.cpp )
endif()
if (WIN32)
  set(libklustersshared_SRCS ${libklustersshared_SRCS} libinqt5/qstandardpaths_win.cpp )
endif()
if (APPLE)
  set(libklustersshared_SRCS ${libklustersshared_SRCS} libinqt5/qstandardpaths_mac.cpp )
endif()

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
       gui/qhelpviewer.h
       shared/channelcolors.h
       shared/itemcolors.h
       shared/types.h
       shared/array.h
       shared/utilities.h
       shared/zoomwindow.h
       #Remove in qt5
       libinqt5/qstandardpaths.h
       )


SET(translate_files
    klustershared.ts
)

if(UPDATE_TRANSLATION)
QT4_CREATE_TRANSLATION(translateSrcs ../translate ${libklustersshared_SRCS} ${translate_files})
endif()

qt4_add_resources(libklustersshared_SRCS gui/libklustersshared-icons.qrc)

add_library (klustersshared SHARED ${libklustersshared_SRCS} ${translateSrcs})
if(QT_QTWEBKIT_FOUND)
     target_link_libraries(klustersshared ${QT_QTWEBKIT_LIBRARY})
endif()

target_link_libraries( klustersshared ${QT_LIBRARIES})

install( TARGETS klustersshared EXPORT LibKlustersSharedLibraryTargets 
    RUNTIME DESTINATION ${BIN_INSTALL_DIR} 
    LIBRARY DESTINATION ${LIB_INSTALL_DIR} 
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR} 
)

install( FILES ${libklustersshared_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR} COMPONENT Devel )


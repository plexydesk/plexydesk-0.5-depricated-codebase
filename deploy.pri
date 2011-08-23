contains(TEMPLATE,.*lib) {
    target.path=$$top_destdirlib
#    TARGET = $$qtLibraryTarget($${TARGET}$${QT_LIBINFIX})
    TARGET = $${TARGET}
} else {
    contains(TEMPLATE,.*app):target.path=$$top_destdir
}

INSTALLS += target

CONFIG += create_prl

LIBS += -L$$top_destdirlib

contains(TEMPLATE,.*lib) {
    target.path=$${OUT_PWD}/build/lib
#    TARGET = $$qtLibraryTarget($${TARGET}$${QT_LIBINFIX})
    TARGET = $${TARGET}
} else {
    contains(TEMPLATE,.*app):target.path=$${OUT_PWD}/build/
}

INSTALLS += target

CONFIG += create_prl

LIBS += -L$${top_destdirlib}

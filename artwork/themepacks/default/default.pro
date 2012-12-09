TEMPLATE = subdirs

THEMEPACK.BACKDROP = backdrop/RadialWave.qml \
                    backdrop/RadialWaveEffect.qml \
                    backdrop/TestWater.qml \
                    backdrop/Water.qml

themepack.backdrop.files = $$THEMEPACK.BACKDROP
themepack.backdrop.path = $${OUT_PWD}/../../../build/share/plexy/themepack/default/backdrop
INSTALLS += themepack.backdrop

THEMEPACK.DIGITALCLOCK.QML = digitalclock/digitalclock.qml
themepack.digitalclock.qml.files = $$THEMEPACK.DIGITALCLOCK.QML
themepack.digitalclock.qml.path = $${OUT_PWD}/../../../build/share/plexy/themepack/default/digitalclock
INSTALLS += themepack.digitalclock.qml

THEMEPACK.DIGITALCLOCK.IMAGES = digitalclock/images/*
themepack.digitalclock.images.files = $$THEMEPACK.DIGITALCLOCK.IMAGES
themepack.digitalclock.images.path = $${OUT_PWD}/../../../build/share/plexy/themepack/default/resources
INSTALLS += themepack.digitalclock.images

THEMEPACK.PHOTO.QML = photo/photoElement.qml
themepack.photo.qml.files = $$THEMEPACK.PHOTO.QML
themepack.photo.qml.path = $${OUT_PWD}/../../../build/share/plexy/themepack/default/photo
INSTALLS += themepack.photo.qml

THEMEPACK.PHOTO.IMAGES = photo/photoElement_images/*
themepack.photo.images.files = $$THEMEPACK.PHOTO.IMAGES
themepack.photo.images.path = $${OUT_PWD}/../../../build/share/plexy/themepack/default/resource
INSTALLS += themepack.photo.images

THEMEPACK.PHOTOFRAME.QML = photoframe/photoframe.qml
themepack.photoframe.qml.files = $$THEMEPACK.PHOTOFRAME.QML
themepack.photoframe.qml.path = $${OUT_PWD}/../../../build/share/plexy/themepack/default/photoframe
INSTALLS += themepack.photoframe.qml

THEMEPACK.PHOTOFRAME.IMAGES = photoframe/frameimages/*
themepack.photoframe.images.files = $$THEMEPACK.PHOTOFRAME.IMAGES
themepack.photoframe.images.path = $${OUT_PWD}/../../../build/share/plexy/themepack/default/resources
INSTALLS += themepack.photoframe.images

THEMEPACK.PHOTOVIEWER.QML = photoviewer/photoviewer.qml
themepack.photoviewer.qml.files = $$THEMEPACK.PHOTOVIEWER.QML
themepack.photoviewer.qml.path = $${OUT_PWD}/../../../build/share/plexy/themepack/default/photoviewer
INSTALLS += themepack.photoviewer.qml

THEMEPACK.PHOTOVIEWER.CORE.QML = photoviewer/PhotoViewerCore/*.qml
themepack.photoviewer.core.qml.files = $$THEMEPACK.PHOTOVIEWER.CORE.QML
themepack.photoviewer.core.qml.path = $${OUT_PWD}/../../../build/share/plexy/themepack/default/photoviewer/PhotoViewerCore
INSTALLS += themepack.photoviewer.core.qml

THEMEPACK.PHOTOVIEWER.CORE.SCRIPTS = photoviewer/PhotoViewerCore/script/script.js
themepack.photoviewer.core.scripts.files = $$THEMEPACK.PHOTOVIEWER.CORE.SCRIPTS
themepack.photoviewer.core.scripts.path = $${OUT_PWD}/../../../build/share/plexy/themepack/default/photoviewer/PhotoViewerCore/script
INSTALLS += themepack.photoviewer.core.scripts

THEMEPACK.PHOTOVIEWER.CORE.IMAGES = photoviewer/PhotoViewerCore/images/*
themepack.photoviewer.core.images.files = $$THEMEPACK.PHOTOVIEWER.CORE.IMAGES
themepack.photoviewer.core.images.path = $${OUT_PWD}/../../../build/share/plexy/themepack/default/resources
INSTALLS += themepack.photoviewer.core.images

THEMEPACK.RESOURCES = resources/*
themepack.resources.files = $$THEMEPACK.RESOURCES
themepack.resources.path = $${OUT_PWD}/../../../build/share/plexy/themepack/default/resources
INSTALLS += themepack.resources

#THEMEPACK.WEATHER.QML = weather/weather.qml
#themepack.weather.qml.files = $$THEMEPACK.WEATHER.QML
#themepack.weather.qml.path = $${OUT_PWD}/../../../build/share/plexy/themepack/default/weather
#INSTALLS += themepack.weather.qml

THEMEPACK.WELCOME.QML = welcome/welcome.qml
themepack.welcome.qml.files = $$THEMEPACK.WELCOME.QML
themepack.welcome.qml.path = $${OUT_PWD}/../../../build/share/plexy/themepack/default/welcome
INSTALLS += themepack.welcome.qml

THEMEPACK.WELCOME.SCRIPTS = welcome/scripts/Scripts.js
themepack.welcome.scripts.files = $$THEMEPACK.WELCOME.SCRIPTS
themepack.welcome.scripts.path = $${OUT_PWD}/../../../build/share/plexy/themepack/default/welcome/scripts
INSTALLS += themepack.welcome.scripts

THEMEPACK.WELCOME.IMAGES = welcome/images/*
themepack.welcome.images.files = $$THEMEPACK.WELCOME.IMAGES
themepack.welcome.images.path = $${OUT_PWD}/../../../build/share/plexy/themepack/default/resources
INSTALLS += themepack.welcome.images

THEMEPACK.MAIN = main.cfg layout.xml
themepack.main.files += $$THEMEPACK.MAIN
themepack.main.path = $${OUT_PWD}/../../../build/share/plexy/themepack/default
INSTALLS += themepack.main

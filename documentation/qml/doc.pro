doc.commands = `qmake -query QT_INSTALL_BINS`/qdoc $$PWD/libhud-qt.qdocconf
doc.files    = $$PWD/html
doc.path     = /usr/share/libhud-qt/doc

QMAKE_EXTRA_TARGETS += doc
INSTALLS += doc

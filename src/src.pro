TEMPLATE = subdirs

include(../config.pri)

SUBDIRS = common

maemo5: {
  SUBDIRS += maps \
             gui \
             tracker
}

linux: {
  SUBDIRS += \
             json \
             http_requests \
             service \
             maps \
	     webside \
             tracker
}

symbian: {
}



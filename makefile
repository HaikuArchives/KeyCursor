# makefile for KeyCursor input Add-On / KeyCursor Preflet

SUBDIRS = \
	preflet \
	src

default .DEFAULT :
	-@for f in $(SUBDIRS) ; do \
		$(MAKE) -C $$f -f makefile $@; \
	done

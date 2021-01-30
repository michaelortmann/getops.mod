# Makefile for src/mod/getops.mod/
# $Id: Makefile,v 1.3 2000/08/25 20:19:16 sup Exp $

srcdir = .


doofus:
	@echo ""
	@echo "Let's try this from the right directory..."
	@echo ""
	@cd ../../../ && make

static: ../getops.o

modules: ../../../getops.so

../getops.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -DMAKING_MODS -c $(srcdir)/getops.c
	@rm -f ../getops.o
	mv getops.o ../

../../../getops.so: ../getops.o
	$(LD) -o ../../../getops.so ../getops.o
	$(STRIP) ../../../getops.so

depend:
	$(CC) $(CFLAGS) $(CPPFLAGS) -MM $(srcdir)/getops.c > .depend

clean:
	@rm -f .depend *.o *.so *~
distclean: clean

#safety hash
../getops.o: .././getops.mod/getops.c ../../../src/mod/module.h \
 ../../../src/main.h ../../../src/lang.h ../../../src/eggdrop.h \
 ../../../src/flags.h ../../../src/proto.h ../../../lush.h \
 ../../../src/misc_file.h ../../../src/cmdt.h ../../../src/tclegg.h \
 ../../../src/tclhash.h ../../../src/chan.h ../../../src/users.h \
 ../../../src/compat/compat.h ../../../src/compat/inet_aton.h \
 ../../../src/compat/snprintf.h ../../../src/compat/memset.h \
 ../../../src/compat/memcpy.h ../../../src/compat/strcasecmp.h \
 ../../../src/mod/modvals.h ../../../src/tandem.h

# Makefile for src/mod/getops.mod/

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
getops.o: .././getops.mod/getops.c .././getops.mod/../module.h \
 ../../../src/main.h ../../../config.h ../../../eggint.h ../../../lush.h \
 ../../../src/lang.h ../../../src/eggdrop.h ../../../src/compat/in6.h \
 ../../../src/flags.h ../../../src/cmdt.h ../../../src/tclegg.h \
 ../../../src/tclhash.h ../../../src/chan.h ../../../src/users.h \
 ../../../src/compat/compat.h ../../../src/compat/snprintf.h \
 ../../../src/compat/strlcpy.h .././getops.mod/../modvals.h \
 ../../../src/tandem.h .././getops.mod/../irc.mod/irc.h \
 .././getops.mod/../server.mod/server.h \
 .././getops.mod/../channels.mod/channels.h .././getops.mod/getops.h \
 .././getops.mod/delay.c .././getops.mod/requests.c \
 .././getops.mod/needbinds.c .././getops.mod/bot.c .././getops.mod/misc.c

AM_YFLAGS = -d
bin_PROGRAMS=rayscript
rayscript_SOURCES= \
					$(top_srcdir)/src/parser/rayscript_lexer.c \
					$(top_srcdir)/src/parser/rayscript_parser.c \
				  	$(top_srcdir)/src/objects/map_object.c \
				  	$(top_srcdir)/src/objects/bool_object.c \
				  	$(top_srcdir)/src/objects/nil_object.c \
				  	$(top_srcdir)/src/objects/type_object.c \
				  	$(top_srcdir)/src/objects/number_object.c \
				  	$(top_srcdir)/src/objects/string_object.c \
				  	$(top_srcdir)/src/objects/function_object.c \
				  	$(top_srcdir)/src/objects/list_object.c \
					$(top_srcdir)/src/objects/object.c \
					$(top_srcdir)/src/vm/vm.c \
					$(top_srcdir)/src/vm/opcode.c \
					$(top_srcdir)/src/parser/visitor.c
rayscript_LDADD=
$(top_srcdir)/src/parser/rayscript_lexer.c: $(top_srcdir)/src/parser/rayscript_lexer.l $(top_srcdir)/src/parser/rayscript_parser.c
	flex -o $(top_srcdir)/src/parser/rayscript_lexer.c $(top_srcdir)/src/parser/rayscript_lexer.l  


$(top_srcdir)/src/parser/rayscript_parser.c: $(top_srcdir)/src/parser/rayscript_parser.y
	bison -Werror --verbose -t --defines=$(top_srcdir)/src/include/rayscript.tab.h -o $(top_srcdir)/src/parser/rayscript_parser.c $(top_srcdir)/src/parser/rayscript_parser.y

debug_lexer: $(top_srcdir)/src/parser/rayscript_lexer.c
	$(CC) -I./src/include -DDEBUG_LEXER src/parser/rayscript_lexer.c -o debug_lexer

AM_CFLAGS=-I$(top_srcdir)/src/include  -std=c99 -Werror `pkg-config --libs --cflags libedit`

export AM_CFLAGS
clean-local: 
	--rm -rf $(top_srcdir)/src/parser/rayscript_lexer.c $(top_srcdir)/src/parser/rayscript_parser.c $(top_srcdir)/src/include/rayscript.tab.h
	test -z "debug_lexer" || rm -f debug_lexer

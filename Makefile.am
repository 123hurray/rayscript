AM_YFLAGS = -d
bin_PROGRAMS=rayscript
rayscript_SOURCES= \
					$(top_srcdir)/src/parser/rayscript_lexer.c \
					$(top_srcdir)/src/parser/rayscript_parser.c \
				  	$(top_srcdir)/src/objects/map_object.c \
					$(top_srcdir)/src/objects/object.c \
					$(top_srcdir)/src/vm/vm.c \
					$(top_srcdir)/src/vm/opcode.c \
					$(top_srcdir)/src/parser/visitor.c
rayscript_LDADD=

$(top_srcdir)/src/parser/rayscript_lexer.c: $(top_srcdir)/src/parser/rayscript_lexer.l $(top_srcdir)/src/parser/rayscript_parser.c
	flex -o $(top_srcdir)/src/parser/rayscript_lexer.c $(top_srcdir)/src/parser/rayscript_lexer.l  


$(top_srcdir)/src/parser/rayscript_parser.c: $(top_srcdir)/src/parser/rayscript_parser.y
	bison --defines=$(top_srcdir)/src/include/rayscript.tab.h -o $(top_srcdir)/src/parser/rayscript_parser.c $(top_srcdir)/src/parser/rayscript_parser.y


AM_CFLAGS=-I$(top_srcdir)/src/include  -std=c99
export AM_CFLAGS
clean-local: 
	--rm -rf $(top_srcdir)/src/parser/rayscript_lexer.c $(top_srcdir)/src/parser/rayscript_parser.c $(top_srcdir)/src/include/rayscript.tab.h

AM_YFLAGS = -d
ACLOCAL_AMFLAGS=-I m4
AUTOMAKE_OPTIONS=foreign subdir-objects
bin_PROGRAMS=rayscript
rayscript_SOURCES= \
					../src/parser/rayscript_lexer.c \
					../src/parser/rayscript_parser.c \
				  	../src/objects/map_object.c \
				  	../src/objects/bool_object.c \
				  	../src/objects/nil_object.c \
				  	../src/objects/type_object.c \
				  	../src/objects/number_object.c \
				  	../src/objects/string_object.c \
				  	../src/objects/function_object.c \
				  	../src/objects/list_object.c \
					../src/objects/object.c \
					../src/vm/vm.c \
					../src/vm/opcode.c \
					../src/parser/visitor.c

if EDITLINE
rayscript_LDFLAGS=`pkg-config --libs --cflags libedit`
rayscript_LDADD=-ledit
endif


rayscript_CFLAGS=-I../src/include  -std=c99 -Werror 

../src/parser/rayscript_lexer.c: ../src/parser/rayscript_lexer.l ../src/parser/rayscript_parser.c
	flex -o ../src/parser/rayscript_lexer.c ../src/parser/rayscript_lexer.l  


../src/parser/rayscript_parser.c: ../src/parser/rayscript_parser.y
	bison -Werror --verbose --defines=../src/include/rayscript.tab.h -o ../src/parser/rayscript_parser.c ../src/parser/rayscript_parser.y

debug_lexer: ../src/parser/rayscript_lexer.c
	$(CC) -I../src/include -DDEBUG_LEXER src/parser/rayscript_lexer.c -o debug_lexer

clean-local: 
	--rm -rf ../src/parser/rayscript_lexer.c ../src/parser/rayscript_parser.c ../src/include/rayscript.tab.h
	test -z "../debug_lexer" || rm -f ../debug_lexer

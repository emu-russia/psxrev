set PATH=c:\lcc\bin

lc -nw main.c lexer.c parser.c context.c runtime.c -o verilike.exe

verilike.exe > out.txt
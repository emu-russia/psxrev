set PATH=c:\tcc

tcc main.c lexer.c parser.c context.c runtime.c -o verilike.exe

verilike.exe > out.txt
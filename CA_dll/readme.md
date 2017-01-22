TDM-GCC
#生成动态链接库
gcc -c CA.c -o CA.o
gcc -shared CA.o -o CA.dll
#或者
gcc -shared CA.c -o CA.dll
#生成测试程序
gcc testCA.c -o testCA.exe CA.dll
#测试
testCA.exe
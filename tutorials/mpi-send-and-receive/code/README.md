Generate byte code from C source 
```
OMPI_CC=clang mpicc -O3 -emit-llvm send_recv_nonblk.c -c -o send_recv_nonblk.bc
```
Apply LLVM pass to byte code and generate transformed byte code
```
opt -load /ssd1/llvm-project/build/lib/LLVMHello.so -hello < send_recv_nonblk.bc > send_recv_nonblk_transformed.bc
```
Convert byte code to object file
```
llc -filetype=obj send_recv_nonblk_transformed.bc
```
Compile object code into executable
```
OMPI_CC=clang mpicc send_recv_nonblk_transformed.o -o send_recv_nonblk_transformed
```
Run the transformed code
```
mpirun -np 2 ./send_recv_nonblk_transformed
```

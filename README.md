### HCMUS OPERATING SYSTEM SUBJECT - K21 - CLC
## COLLABORATORS:
- Trần Tùng Lâm - 21127337
- Trần Bình Kha - 21127065
- Lê Vũ Ngân Lam - 21127334

## TODO:
- Modify all the exception call from machine, following by the requirements of the subject: ./NachOS-4.0/code/userprog/exception.cc
- For Implementing SystemCallFunction: ./NachOS-4.0/code/userprog/ksyscall.h
- For Testing: ./NachOS-4.0/code/test
    - Make sure create a `file`.c and implementing code into it using the syscall function that has just been implemented from the file above.
    - Make sure to modify MakeFile to compile the code: PROGRAMS = add halt shell matmult sort segments create (more functions)...
    - Run commands: make all
- Make sure to run this command after making any changes: cd ./NachOS-4.0/code/build.linux && make depend && make. If there is any error, try to fix it and recompile the code.
- Run this command to test the program: cd ./NachOS-4.0/code/test && ../build.linux/nachos -x `name_of_the_file` (Ex: ./create) and checking the result. Ex: For Create Syscall function, it will creates a file named 'file.txt', if that file is existed in test folder then our program works succesfully!

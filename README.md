# HCMUS OPERATING SYSTEM SUBJECT - K21 - CLC
## COLLABORATORS:
- Trần Tùng Lâm - 21127337
- Trần Bình Kha - 21127065
- Lê Vũ Ngân Lam - 21127334

## INSTRUCTIONS:
- Modify all the exception call from machine, following by the requirements of the subject: ./NachOS-4.0/code/userprog/exception.cc
- For Implementing SystemCallFunction: `./NachOS-4.0/code/userprog/ksyscall.h`
- For Testing: `./NachOS-4.0/code/test`
    - Make sure create a `file`.c and implementing code into it using the syscall function that has just been implemented from the file above.
    - Make sure to modify MakeFile to compile the code: PROGRAMS = add halt shell matmult sort segments create (more functions)...
    - Run commands: make all
    - Make sure to check `start.S` file, if it doesn't not have your syscall, add it up.
- Make sure to run this command after making any changes: `cd ./NachOS-4.0/code/build.linux && make depend && make`. If there is any error, try to fix it and recompile the code.
- Run this command to test the program: `cd ./NachOS-4.0/code/test` and `../build.linux/nachos -x \`name_of_the_file`\` (Ex: create, halt, add, etc.) and checking the result. Ex: For Create Syscall function, it will creates a file named 'file.txt', if that file is existed in test folder then our program works succesfully!

## HOW TO INSTALL WSL2 ON WINDOWS:
1. Open cmd or powershell with admin and run this: `wsl --install`. If you are already turn on the Window subsystem for linux then run this: `wsl --install -d ubuntu`
2. Follow the instructions on this link: 'https://www.fit.hcmus.edu.vn/~ntquan/os/setup_nachos.html' to step 3 only
3. Clone this repo and start working on this project

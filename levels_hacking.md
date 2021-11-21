level 1: entering command line argument argv[1] with not less than 128 symbols in length, will overflow char type, because char type has values in segment [-128, 127]
string: "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
level 2: entering command line arguments like 4294967196 /*UINT_MAX - 99*/ 10 10 will overflow unsigned int type to value 0
level 3: entering command line arguments like -1 is less than 200, but casting to unsigned int type value of variable i will be UINT_MAX
level 4: entering command line arguments like 4294967294 /*UINT_MAX - 1*/ will overflow variable s to zero
level 5: entering buf variable with 17 characters length and more will overflow system stack, in which earlier was created variable a, so a will have a value of 16th character of entered string
level 6: entering command line arguments like System32 will call cmd.exe succesfully.
level 7: entering command line arguments like a sequence of 16 characters will move '\0' symbol to the last byte of variable b, so that values of b will differ in one byte (255)
level 8: entering command line arguments like "%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x"
level 9: entering command line arguments like ccccc cccccccccccccccccccccccccccccccc will call overflow of key size (37), because of "key=" and 32 'c's and 2 '\0' so ASCII-code of 'c' will be written to i
level 10: run .exe file by passing to command line the absolute path to .exe file, so 
buf[16] will be written to i due to one stack
level 11: entering command line argumenrts like cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc1234DCBA 
(length == 88), so it will pass 1234DCBA to variables saved_ebp and saved_eip, so DCBA will be bytes (from right to left) of saved_eip, so saved_eip == 0x41424344
level 12: entering a sequence of 87 symbols, 3 last of which are symbols with codes 15, 16, 64 (ctrl+O, ctrl+P, @). By running level13.exe recognized the adress of function unreachable is 0x40100f, and after overflowing buf size in the stack is the address of unreachable

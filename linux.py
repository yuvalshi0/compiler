
import os
import subprocess
os.chdir("/home/yuvals/Study/Compilation/")
files = os.listdir()
c_files = []
for file in files:
    if os.path.splitext(file)[1] == '.lex' or os.path.splitext(file)[1] == '.l':
        lex_file = file
    if os.path.splitext(file)[1] == '.c':
        c_files.append(file)


if lex_file != '':
    flex_command = "flex " + lex_file
    cc_command = "gcc " + ' '.join(c_files) + " -lfl"

    print("\033[0;37;40mRunning: \033[1;32;40m" +
          flex_command + "\033[0;37;40m...")
    os.system(flex_command)
    print("Running: \033[1;32;40m" + cc_command + "\033[0;37;40m...")
    os.system(cc_command)
print("If, not errors are presented, you may run test your code with the command \"./a.out\"")

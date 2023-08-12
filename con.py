#!/usr/bin/python3

import cmd
import sys

class Helloworld(cmd.Cmd):
    "simple command processor example"
    prompt = '(hbnb) '

    def do_greet(self, person):
        "greet person"
        if person:
            print("Good day", person)
        else:
            print("hello")

    def do_EOF(self, line):
        "End of file"
        print('Ctrl D ending......')
        return True

    def do_shell(self, line):
        print('am shell')

    def postloop(self):
        print()

if __name__ == '__main__':
    helloworld = Helloworld()
    
    if len(sys.argv) > 1:
        for line in sys.stdin:
            helloworld.onecmd(line.strip())
    else:
        helloworld.cmdloop()

#!/usr/bin/python3

import cmd
import json
import re

import models

from models import FileStorage, storage
from models.base_model import BaseModel
from models.user import User
from models.review import Review
from models.place import Place
from models.city import City
from models.amenity import Amenity
from models.state import State

class HBNBCommand(cmd.Cmd):
    prompt = '(hbnb) '
    __valid_classes = {
        "BaseModel", "User", "State", "City",
        "Place", "Amenity", "Review"
    }

    def do_quit(self, line):
        "Quit command to exit the program"
        return True
    def do_EOF(self, line):
        "exit the program using Ctrl + d"
        return True

    def emptyline(self):
        '''dont execute anything when user
           press enter an empty line
        '''
        pass
    def do_create(self, line):
        "Create a new instance of the class called"
        if not line:
            print('** class name missing **')
        else:
            if line in globals() and isinstance(globals()[line], type):
                b = BaseModel()
                b.save()
                models.storage.reload()
                print(b.id)
            else:
                print("** class doesn't exist **")

    def do_show(self, args):
        '''Prints the string representation of a specific instance
           Usage: show <class name> <id>
        '''
        strings = args.split()
        if len(strings) == 0:
            print("** class name missing **")
        elif strings[0] not in HBNBCommand.__valid_classes:
            print("** class doesn't exist **")
        elif len(strings) == 1:
            print("** instance id missing **")
        else:
            obj = storage.all()
            key_value = strings[0] + '.' + strings[1]
            if key_value in obj:
                print(obj[key_value])
            else:
                print("** no instance found **")

    def do_destroy(self, line):
        " Deletes an instance based on the class name and id"
        data = models.storage.all()
        if not line:
            print('** class name missing **')
        else:
            try:
                args = line.split()
                arg1, arg2 = args
            except ValueError:
                print('** instance id missing **')
                return
            if arg1 in HBNBCommand.__valid_classes:
                key = f"{arg1}.{arg2}"
                if key in data.keys():
                    data.pop(key, None)
                    models.storage.save()
                    models.storage.reload()
                else:
                    print('** no instance found **')
            else:
                print("** class doesn't exist **")

    def default(self, item):
        objects = models.storage.all()
        new_list =[]

        keys = re.split(r'\.|\(\)|\(|\)|,', item)
        if keys[0] in self.__valid_classes and keys[1] == "all":
            for obj in objects.values():
                if obj.__class__.__name__ == keys[0]:
                    new_list.append(obj.__str__())
            formatted_output = ", ".join(new_list)
            print("[" + formatted_output + "]")
        elif keys[0] in self.__valid_classes and keys[1] == "count":
            class_name = keys[0]
            count = sum(1 for obj in objects.values() if obj.__class__.__name__ == class_name)
            print(count)
        elif keys[0] in self.__valid_classes and keys[1] == "show":
            s_id = keys[2].lstrip('"').rstrip('"')
            key_value = f'{keys[0]}.{s_id}'
            if key_value in objects:
                print(objects[key_value])
            else:
                print('** no instance found **')
        elif keys[0] in self.__valid_classes and keys[1] == "destroy":
            s_id = keys[2].lstrip('"').rstrip('"')
            key_value = f'{keys[0]}.{s_id}'
            if key_value in objects:
                objects.pop(key_value, None)
                models.storage.save()
                models.storage.reload()
            else:
                print('** no instance found **')
        elif keys[0] in self.__valid_classes and keys[1] == "update":
            s_id = keys[2].lstrip('"').rstrip('"')
            key_value = f'{keys[0]}.{s_id}'
            # Extract every word without double parentheses
            filtered_words = keys[3].strip('.()"')

            print(filtered_words)

    def do_all(self, args):
        '''Print a string representation of all instances
           Usage: all <class name>
        '''
        args = args.split()
        objects = models.storage.all()
        new_list = []

        if len(args) == 0:
            for obj in objects.values():
                new_list.append(obj.__str__())
            print(new_list)
        elif args[0] not in HBNBCommand.__valid_classes:
            print("** class doesn't exist **")
        else:
            for obj in objects.values():
                if obj.__class__.__name__ == args[0]:
                    new_list.append(obj.__str__())
            print(new_list)

    def do_update(self, args):
        '''update an instance
           Usage update <class name> <id> <attribute name> "<attribute value>"
        '''

        if not args:
            print("** class name missing **")
            return False
        objects = models.storage.all()
        args = args.split(" ")


        if args[0] not in HBNBCommand.__valid_classes:
            print("** class doesn't exist **")
        elif len(args) == 1:
            print("** instance id missing **")
        elif len(args) == 2:
            print("** attribute name missing **")
        elif len(args) == 3:
            print("** value missing **")
        elif args[2] == "id" or args[2] == "created_at" or args[2] == "updated_at":
            return
        else:
            key_find = args[0] + '.' + args[1]
            obj = objects.get(key_find, None)

            if not obj:
                print("** no instance found **")
                return
            if args[2] in obj.__class__.__dict__.keys():
                valtype = type(obj.__class__.__dict__[args[2]])
                obj.__dict__[args[2]] = valtype(args[3].lstrip('"').rstrip('"'))
                models.storage.save()
                models.storage.reload()
            else:
                obj.__dict__[args[2]] = args[3].lstrip('"').rstrip('"')
            models.storage.save()
            models.storage.reload()





if __name__ == '__main__':
    HBNBCommand().cmdloop()

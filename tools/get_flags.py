#!/usr/bin/env python2

import json
import os
import sys
import re

from string import Template

IGNORED_FLAGS = ['-o', '-c']

try:
    base_dir = os.path.abspath(sys.argv[1])
    with open(os.path.join(base_dir, 'compile_commands.json')) as f:
        db = json.load(f)
        flags_dict = {'-x':'c++', '-std=c++17':''}
        for entry in db:
            args = entry['command']
            prev = None
            for arg in args.split():
                if prev is not None:
                    if arg.startswith('-'):
                        if prev not in IGNORED_FLAGS:
                            flags_dict[prev] = ""
                            prev = arg
                    else:
                        if prev not in IGNORED_FLAGS:
                            flags_dict[prev] = arg
                            prev = None
                elif arg.startswith('-'):
                    prev = arg

        # Defaul to c++17 only
        flags_dict.pop("-std=c++11", None)
        flags_dict.pop("-std=c++14", None)
        flags_dict.pop("-std=gnu++11", None)
        flags_dict.pop("-std=gnu++14", None)

        final_flags = "\n".join(['    \'{0}\',\n    \'{1}\','.format(key, value) for (key, value) in sorted(flags_dict.items())])

        final_flags = re.sub("    \'\',\n|    \'\',", "", final_flags)

        # for key, val in sorted(flags_dict.items()):
            # print key, "=>", val
        if os.path.exists(os.path.join(base_dir, '.ycm_extra_conf.py')):
            print(".ycm_extra_conf already exists. Overwrite? [y/N] "),
            response = sys.stdin.readline().strip().lower()
            if(response != "y" and response != "yes"):
                sys.exit()

        print("Now writing .ycm_extra_conf.py in '{}'".format(base_dir))

        with open(os.path.join(base_dir, 'ycm_extra_conf.template')) as templ:
            with open(os.path.join(base_dir, '.ycm_extra_conf.py'), 'w') as outf:
                new_template = Template(templ.read())
                new_file = new_template.substitute(flags=final_flags, database=os.path.join(base_dir, ''))
                outf.write(new_file)
                outf.close()

except(IOError, OSError):
    print IOError.message
    print OSError.message
    print "Bad arguments\nUsage:\n\tpython /some/path/get_flags.py directory_containing compile_commands.json"

#!/usr/bin/env python

import errno
import os
import subprocess
import sys

CMAKE = "cmake"
MAKE = "make"
CMAKE_GENERATOR = "Unix Makefiles"
CONFIGS = {}
script_dir = os.path.dirname(__file__)
DEFAULT_CONFIG_NAME = "release32"
DEBUG_CONFIG_NAME = "debug32"

CONFIGS[DEBUG_CONFIG_NAME] = ["-DCMAKE_BUILD_TYPE=Debug"]
CONFIGS[DEFAULT_CONFIG_NAME]= ["-DCMAKE_BUILD_TYPE=Release"]


def get_project_root_path():
    import __main__
    return os.path.dirname(__main__.__file__)


def get_builds_path():
    return os.path.join(get_project_root_path(), "builds")

def get_src_path():
    return os.path.join(get_project_root_path(), "src")

def get_build_path(config_name):
    return os.path.join(get_builds_path(), config_name)


def try_run(cmd, cwd):
    try:
        print(cmd,cwd)
        subprocess.check_call(cmd, cwd=cwd)
    except OSError as exc:
        if exc.errno == errno.ENOENT:
            print("Could not find '%s' on your PATH." %cmd[0])
            sys.exit(1)
        else:
            raise


def build(config_name, cmake_parameters,make_parameters):
    print("Building configuration " + config_name)
    build_path = get_build_path(config_name)
    rel_src_path = os.path.relpath(get_src_path(), build_path)
    try:
        os.makedirs(build_path)
    except OSError as exc:
        if exc.errno == errno.EEXIST and os.path.isdir(build_path):
            pass
        else:
            raise

    try_run([CMAKE, "-G", CMAKE_GENERATOR] + cmake_parameters + [rel_src_path],
            cwd=build_path)


    try_run([MAKE] + make_parameters, cwd=build_path)

    print("Built configuration " + config_name + " successfully")


def main():
    make_parameters = []
    config_names = set()
    for arg in sys.argv[1:]:
        if arg == "--debug":
            config_names.add(DEBUG_CONFIG_NAME)

    if not config_names:
        config_names.add(DEFAULT_CONFIG_NAME)

    for config_name in config_names:
        build(config_name, CONFIGS[config_name],make_parameters)


if __name__ == "__main__":
    main()

#! /usr/bin/env python
# -*- coding: utf-8 -*-

from Cheetah.Template import Template
import os
import sys

# defines the benchmark that is used for the experiment (must be the name of a
# folder in /benchmarkfiles)
benchmark="Prost"

# defines which queue to use for one task. 
queue = "meta_core.q"

# defines which executable to use
executable = "builds/release32/bin/EvmddBenchmark"

# defines the timeout for one taks. The time format is
# "hours:minutes:seconds", eg, a value of "0:30:00" sets the timeout
# to 30 minutes. If timout is set to None, then there is no timeout.
timeout = "0:30:00"

# defines the maximum amount of available memory for one task. The
# value's format is either "<mem>M" or "<mem>G", where <mem> is an
# integer number, M stands for MByte and G for GByte. If memout is
# None, then there is no memory bound.
memout = "4G"

revision = "203f615"

TASK_TEMPLATE = "./%(executable)s benchmarkfiles/%(benchmark)s/%(instance)s"

def create_tasks(filename, instances):
    tasks = []

    print len(instances)
    print instances
    
    for instance in sorted(instances):
        task = TASK_TEMPLATE % dict(executable = executable, benchmark = benchmark, instance=instance)
        tasks.append(task)


    template = Template(file='job.tmpl',
                        searchList=[{'tasks'    : tasks,
                                     'queue'    : queue,
                                     'timeout'  : timeout,
                                     'memout'   : memout}])    
    f = file(filename, 'w')
    f.write(str(template))
    f.close()

if __name__ == '__main__':
    if len(sys.argv) > 1:
        print >> sys.stderr, "Usage: create-jobs.py"
        exit()
    instances = os.listdir("../benchmarkfiles/"+benchmark+"/")
    create_tasks("../evmdd_benchmark.q", instances)

import os
import subprocess
import random
import pickle
import sys
# If we give the seeds file of a previous execution it will repeate

output_folder ='output'
dataset_folder = 'datasets'


def run_instance(path, seed):
    instance = subprocess.run(['build/bin/main', path, str(seed)], capture_output=True, text= True)
    return instance.stdout

def next_seed(is_random, instance_name, seeds):#a random seed if is_random == True or the seed of a previous execution
    if is_random:
        seed = random.randint(0, 2147483647)
        seeds[instance_name] = seed
        return seed
    else:
        try:#in case we give a seeds dict of an execution with a different set of instances...
            return seeds[instance_name]
        except:
            return random.randint(0, 2147483647)

def run_all(random_seeds, seeds):#random_seeds == Tres seeds will randomly generatared otherwise I'm assuming that seeds is a properly loaded map of seeds
    for path, folders, files in os.walk(dataset_folder):
        for filename in files:
            if filename.find("Init") < 0:
                seed = next_seed(random_seeds, filename, seeds)
                output = run_instance(os.path.join(path, filename), seed)
                file_prefix = path.removeprefix(dataset_folder+'/').replace("/","_")
                file_sufix = filename.removesuffix(".txt")
                with open(output_folder+"/{0}-{1}.csv".format(file_prefix, file_sufix),'w',newline='') as file:
                   file.write(output)
    
if not os.path.exists(output_folder):
    os.makedirs(output_folder)


subprocess.run('make')
seeds = {}
if len(sys.argv) < 2:#if the seed file is not provided, 
    with open(output_folder+'/seeds.out', 'wb') as seeds_file:
        run_all(random_seeds=True, seeds=seeds)  
        pickle.dump(seeds, seeds_file)
else:
    try:
        with open(sys.argv[1], "rb") as seeds_file:
            seeds = pickle.load(seeds_file)
    except:
        print('Seeds file not valid')
    run_all(random_seeds=False, seeds=seeds)

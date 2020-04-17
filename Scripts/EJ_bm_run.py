

from os import listdir, makedirs
from os.path import isfile, join, exists
from subprocess import Popen, CREATE_NEW_CONSOLE
from datetime import datetime
from shutil import rmtree


base_json = r"""
{
    "m_inputScene": "@",
    "m_outputLog": "$",
    "m_algorithm": "%",
    "m_algorithm_prettyName": "~",
    "&": "£"
}
"""

def EJ_generate_jsons(batch_name, results_folder, tests_folder, scenes_folder, algorithm, additionalProp = "", aditionalPropValues = [""]):
  
  # finding all scene files
    scenes = [f for f in listdir(scenes_folder) if isfile(join(scenes_folder, f))]

    # creating tests and results folders
 #  if batch_name != "":
 #      tests_folder = join(root_folder, batch_name)
 #      if (exists(tests_folder)):
 #          rmtree(tests_folder) # in case it already existed
 #  else:
 #      tests_folder = join(root_folder, "Tests " + datetime.now().strftime('%Y-%m-%d_%H-%M-%S' + '/'))

    #results_folder = join(tests_folder, "Results/")
   # makedirs(tests_folder)
    #makedirs(results_folder)
    # generating all jsons for scene-algorithm pairs
    for s in scenes:
        if additionalProp == "":
            task = s + "_" + algorithm 
            data = base_json
            data = data.replace("@", join(scenes_folder, s))
            data = data.replace("$", join(results_folder, task + batch_name + "_results.json"))
            data = data.replace("%", algorithm)
            data = data.replace("~", algorithm)
            
            
            data = data.replace("&", "NoAdditionalProperty")
            data = data.replace('"£"', '""')
            with open(join(tests_folder, task + batch_name+ ".json"), 'w') as json:
                json.write(data)	
        else:
            for value in aditionalPropValues:
                alg_prettyName = algorithm + "_" + additionalProp + "_" + str(value)
                task = s + "_" + alg_prettyName + ".json"
                data = base_json
                data = data.replace("@", join(scenes_folder, s))
                data = data.replace("$", join(results_folder, task + batch_name + "_results.json"))
                data = data.replace("%", algorithm)
                data = data.replace("~", alg_prettyName)
                
                
                data = data.replace("&", additionalProp)
                data = data.replace('"£"', str(value))
                
                with open(join(tests_folder, task + batch_name), 'w') as json:
                    json.write(data)
                
    return tests_folder
    

def EJ_run_algorithms(tests_folder, broadmark_bin):
    # display header
    process = Popen([broadmark_bin])
    process.wait()
    
    # fetching all generated jsons and executing them
    tests = [f for f in listdir(tests_folder) if isfile(join(tests_folder, f))]
    count = 1
    for t in tests:
        print("Test " + str(count) + " of " + str(len(tests)))
        
        args = [broadmark_bin, join(tests_folder, t)]
        process = Popen(args)
        process.wait()
        count = count + 1

    results_folder = join(tests_folder, "Results/")
    return results_folder
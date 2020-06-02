
from os.path import isfile, join, basename,exists
import os
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

def EJ_generate_jsons( results_folder, tests_folder, pathOB, algorithm, additionalProp = "", aditionalPropValues = [""]):
  
  
  
  
    #Generar kör filer för T1, T2, ... , TN
    for t in os.listdir(pathOB) :
        if (t[0] != "_"):
            pathT         = pathOB + t + '/'
            scenes_folder = pathT + "Recordings_AABBs" + '/'
            batch_name     = "_"+t

            scenes = [f for f in listdir(scenes_folder) if isfile(join(scenes_folder, f))]
            
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
    

def EJ_run_algorithms(tests_folder, broadmark_bin, testNumer,numberOfTest):
    # display header
    process = Popen([broadmark_bin])
    process.wait()
    
    # fetching all generated jsons and executing them
    tests = [f for f in listdir(tests_folder) if isfile(join(tests_folder, f))]
    count = 1
    for t in tests:
        print("Test " + str(testNumer) + " av " + str(numberOfTest))
        args    = [broadmark_bin, join(tests_folder, t)]
        process = Popen(args)
        process.wait()
        testNumer = testNumer+1
    results_folder = join(tests_folder, "Results/")
    return results_folder, testNumer
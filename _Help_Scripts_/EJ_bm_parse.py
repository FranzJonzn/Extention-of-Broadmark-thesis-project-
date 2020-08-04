
from os import listdir, makedirs
from os.path import isfile, join, basename
from pandas.io.json import json_normalize
import json
import pandas as pd
import numpy as np

# This scripts loads all json data generated at 'results_folder' and feeds it to
# pandas' dataframes. We do so in four layouts, for convenience:
#       Basic Table
#       Basic Table (described)
#       Multi-Index
#       Multi-Index (described)


# configurable parameters

base_json = r"""
{
    "Frames": @,
    "Settings":{ 
        "m_GPUSap_KernelType": &,
        "m_Grid_ObjectsPerCell": $,
        "m_KD_ObjectsPerLeaf": ?,
        "m_Tracy_ObjectsPerCell": %,
        "m_algorithm": "+",
        "m_algorithm_prettyName": "-",
        "m_inputScene": "=",
        "m_margin": £,
        "m_numThreads": !,
        "m_numberOfObjects": *,
		
		"m_procentStatic": ¤,
        "m_averageFrom": ¨
		}
}
"""


def EJ_calcAverage(results_folder, main_out_folder, tests_folder, procent_static):
	# reads all test resulsts
	results     = [f for f in listdir(results_folder) if isfile(join(results_folder, f)) and f.endswith('.json')]
	frameA      = pd.DataFrame()
	averageData = base_json	

	for r in results:
		with (open(join(results_folder, r), "r")) as file:
			data = json.load(file)	
			# loads the data into a dataframe and store the "Total" column
			df 						     = json_normalize(data["Frames"])				
			df 							 = pd.DataFrame(data["Frames"])	
		
			frameA = pd.concat((frameA, df))
			

	frameData   = EJ_formate_Frame_Data(frameA)
	averageData = averageData.replace("@", frameData)
	averageData = EJ_formate_setting_Data(join(results_folder, results[0]), averageData, procent_static,str(len(results)))
	
	name = EJ_formate_name_Data(results[0],results_folder,procent_static)	
     
	#To local results
	average_results_folder = tests_folder +"_Average_Result/"	
	makedirs(average_results_folder)	
	with open(join(average_results_folder, name), 'w') as rf:
		rf.write(averageData)

	#to main results
	with open(join(main_out_folder, name), 'w') as rf:
		rf.write(averageData)
	

def EJ_formate_Frame_Data(frameA):
	frameA_in   = frameA.groupby(frameA.index)
	frameA_mean = frameA_in.mean()
	frameData   = frameA_mean.to_json(orient='records')
	frameData   = frameData[1:];frameData = frameData[:-1]
	frameData   = frameData[1:];frameData = frameData[:-1]
	frameWdata  = frameData.split('{')

	frameResData = ""
	for part in frameWdata:
		part          = part.split(',')#; part = + part[1] +",\n"+ part[2] +",\n"+ part[3] +",\n"+ part[4]
		partTemp      = "            "+part[0] +",\n"
		partTemp     += "            "+part[1] +",\n"
		partTemp     += "            "+part[2] +",\n"
		partTemp     += "            "+part[3] +",\n"
		partTemp     += "            "+part[4]
		part          = "\n        {\n"+partTemp
		part          = part.split('}'); part = part[0] +"\n        },"	
		frameResData += part
		
	
	frameResData = frameResData[:-1]
	frameResData = "["+frameResData+"\n    ]"
	return frameResData	
	
def EJ_formate_setting_Data(result, averageData, procent_static, number_of_teast):
	with (open(result))as file:
		# Copy the informaton from settion to the average
		data = json.load(file)
		averageData = averageData.replace("&",str(data["Settings"]["m_GPUSap_KernelType"]))
		averageData = averageData.replace("$",str(data["Settings"]["m_Grid_ObjectsPerCell"]))
		averageData = averageData.replace("?",str(data["Settings"]["m_KD_ObjectsPerLeaf"]))
		averageData = averageData.replace("%",str(data["Settings"]["m_Tracy_ObjectsPerCell"]))
		averageData = averageData.replace("+",data["Settings"]["m_algorithm"])
		averageData = averageData.replace("-",data["Settings"]["m_algorithm_prettyName"])
		averageData = averageData.replace("=",data["Settings"]["m_inputScene"])
		value       = str(data["Settings"]["m_margin"])
		value       = value.replace("\'", "\"")
		averageData = averageData.replace("£",value)
		averageData = averageData.replace("!",str(data["Settings"]["m_numThreads"]))
		averageData = averageData.replace("*",str(data["Settings"]["m_numberOfObjects"]))
		averageData = averageData.replace("¤",procent_static)
		averageData = averageData.replace("¨",number_of_teast)
		
	return averageData	
	
def EJ_formate_name_Data(results_name, results_folder, procent_static):
	name = " "
	with (open( join(results_folder, results_name)))as file:
		data = json.load(file)
		name = results_name.split(".")
		name = name[0] + "_Resultat.json"
		name = name.split(' ')
		name = name[0] + ' ' + data["Settings"]["m_algorithm_prettyName"] + ' ' + name[1] + ' PS' + procent_static + ' ' + name[2] + ' ' + name[3]
	return name	
	
	
# == File hiarki ==
# 	_procent
#		simulation
#			xs_xe_xi
#				Nx
#					objekt
#						_Testning
#							AlgoritmX
#								_Average_Results
#								results
#						Tx
#							Recordings_AABBs
#							Recordings_PositionRotation

def EJ_process_results(results_folder):
							


    # reads all test resulsts
    results = [f for f in listdir(results_folder) if isfile(join(results_folder, f)) and f.endswith('.json')]
    frame = pd.DataFrame() # keeps a master dataframe to store all algorithms
    for r in results:
        with (open(join(results_folder, r), "r")) as file:
  
            data = json.load(file)
    
            # we are interested on the scene's simple name, the algorithm and the n of objects
            scene     = basename(data["Settings"]["m_inputScene"]).split(' '); scene = scene[0] + ' ' + scene[1]
            algorithm = data["Settings"]["m_algorithm_prettyName"]
            n         = data["Settings"]["m_numberOfObjects"]
            ps        = "ps " + str(data["Settings"]["m_procentStatic"])
            print(ps)
            # loads the data into a dataframe and store the "Total" column
            df = json_normalize(data["Frames"])
            df = pd.DataFrame(data["Frames"])
            #frame[(scene, algorithm, ps, n, )] = df["Total"]
            frame[(scene, ps, n, algorithm)] = df["Total"]
        
    
    
    
    # The master dataframe ready for use
    # Columns: (scene, n, algorithm)
    # Rows: frame_number
    main_frame = frame
    main_frame.to_csv(join(results_folder, "main_frame.csv"), sep=';')
    main_frame.to_excel(join(results_folder, "main_frame.xlsx"))
    main_frame.to_pickle(join(results_folder, "main_frame.pickle"))
    print("_main_frame written!")
  
    
    # The master dataframe in described form
    # Columns: (scene, n, algorithm)
    # Rows: frame_number
    described_frame = frame.describe()
    described_frame.to_csv(join(results_folder, "main_described_frame.csv"), sep=';')
    described_frame.to_excel(join(results_folder, "main_described_frame.xlsx"))
    described_frame.to_pickle(join(results_folder, "main_described_frame.pickle"))
    print("_main_described_frame written!")
    
    
    # Creates a multi-indexed dataframe grouping algorithms by scene then by n
    # Columns: frame_number
    # Rows: Scene > N > Algorithm
    index = pd.MultiIndex.from_tuples(frame.columns)
    mi_frame = frame.transpose()
    mi_frame = mi_frame.set_index(index)

    labels = (mi_frame.columns.to_series() / 50).astype(int)
    mi_frame = mi_frame.groupby(labels, axis=1).mean()
    
    mi_frame.to_csv(join(results_folder, "multi_index_frame.csv"), sep=';')
    mi_frame.to_excel(join(results_folder, "multi_index_frame.xlsx"))
    mi_frame.to_pickle(join(results_folder, "multi_index_frame.pickle"))
    print("_multi_index_frame written!")
    
    
    # Creates a multi-indexed dataframe of described data
    # Columns: count, mean, std, min, 25%, 50%, 75%, max
    # Rows: Scene > N > Algorithm
    index = pd.MultiIndex.from_tuples(frame.columns)
    dmi_frame = frame.describe().transpose()
    dmi_frame = dmi_frame.set_index(index)
    dmi_frame.to_csv(join(results_folder, "multi_index_described_frame.csv"), sep=';')
    dmi_frame.to_excel(join(results_folder, "multi_index_described_frame.xlsx"))
    dmi_frame.to_pickle(join(results_folder, "multi_index_described_frame.pickle"))
    print("_multi_index_described_frame written!")
 



#Används inte längre ingorera
 #frame[(scene, ps, n, algorithm)] = df["Total"]

  #  lines_frame = dmi_frame["mean"].transpose()
  #  lines_frame = lines_frame.reset_index()
  #  lines_frame = pd.pivot_table(lines_frame, values="mean", index=["level_2","level_1"], columns="level_3") #lines_frame = pd.pivot_table(lines_frame, values="mean", index=["level_1","level_2"], columns="level_3")
  #  lines_frame.to_csv(join(results_folder, "lines_frame.csv"), sep=';')
  #  lines_frame.to_excel(join(results_folder, "lines_frame.xlsx"))
  #  lines_frame.to_pickle(join(results_folder, "lines_frame.pickle"))
  #  print("lines_frame written!")

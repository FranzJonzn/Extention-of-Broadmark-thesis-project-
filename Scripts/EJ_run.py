from os import listdir, makedirs
from os.path import isfile, join, basename,exists
import os

from EJ_bm_run import EJ_generate_jsons, EJ_run_algorithms
from EJ_bm_parse import EJ_calcAverage,EJ_process_results


from os import listdir
from os.path import isfile, join, basename
from pandas.io.json import json_normalize
import json
import pandas as pd
import numpy as np
from shutil import rmtree


batch_run_name = "TEST/" # use this to name this testing session. If blank, a auto-generated timestamped name will be used
broadmark_bin = "D:/SKOL_ARBETE/Kurser/ar_3/Vt/Ex_Jobb/BroadMarker/Broadmark-master/Broadmark/bin64/Broadmark_Release_x64.exe"
scenes_folder = "C:/Users/Franz/Desktop/ny3"#"D:/SKOL_ARBETE/Kurser/ar_3/Vt/Ex_Jobb/BroadMarker/Simulations/Sim/Recordings_AABBs/"
output_folder = ""#"D:/SKOL_ARBETE/Kurser/ar_3/Vt/Ex_Jobb/BroadMarker/_Resultat/"
algorithms = ["KD", "CGAL"]#["KD", "CGAL", "GPU_Grid"]
#algorithms = ["SAP", "SAP_AVX_Parallel", "DBVT F", "DBVT D", "Tracy", "Tracy_Parallel", "CGAL", "Grid_3D", "Grid_3D_SAP", "Grid_3D_Parallel", "Grid_3D_SAP_Parallel", "KD", "GPU_Grid", "GPU_LBVH", "GPU_SAP"]
additionalProperty = ""
additionaPropertyValues = []

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







root = scenes_folder


if(output_folder == ""):
	output_folder = scenes_folder +"/_Results/"
	
if (exists(output_folder)):
	rmtree(output_folder) # in case it already existed	
makedirs(output_folder)

for ps in os.listdir(root):
	if(ps[0] != "_"):
		pathPS = root + '/' + ps + '/'
		for sim in os.listdir(pathPS):
			pathSim = pathPS +  sim + '/'
			for inte in os.listdir(pathSim):
			
				pathInte = pathSim + inte + '/'
				for n in os.listdir(pathInte):
					pathN = pathInte +  n + '/'
					for ob in os.listdir(pathN):
						pathOB  = pathN +  ob + '/'
						pathRes = pathOB + "_Testning/"
				
						if (exists(pathRes)):
							rmtree(pathRes) # in case it already existed
						makedirs(pathRes)
						for alg in algorithms:
							pathalg     = pathRes + alg + '/'
							pathResult  = pathalg + "results/"
		
							makedirs(pathalg)
							makedirs(pathResult)
	
							for t in os.listdir(pathOB) :
								if (t[0] != "_"):
									pathT         = pathOB + t + '/'
									scenes_folder = pathT + "Recordings_AABBs" + '/'	
									test_name     = "_"+t
									tests_folder  = EJ_generate_jsons(test_name, pathResult, pathalg, scenes_folder, alg, additionalProperty, additionaPropertyValues)
									results_folder = EJ_run_algorithms(tests_folder, broadmark_bin)
									
							proStat = ps.split('_')		
							EJ_calcAverage(pathResult, output_folder, pathalg, proStat[0])
						
EJ_process_results(output_folder)
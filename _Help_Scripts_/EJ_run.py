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

# == File hiarki ==
#Root
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


broadmark_bin = "D:/SKOL_ARBETE/Kurser/ar_3/Vt/Ex_Jobb/_EX_JOBB_Broadmark_REPO/Broadmark/bin64/Broadmark_Release_x64.exe" #<= ska peka på broadmark .exe filen
scenes_folder = "C:/Users/Franz/Desktop/simForB"                                                                          #<= ska peka på rooten av hierakin som önskas testas
output_folder = ""                                                                                                        #<= alternativ att välja var _Results ska sparas, om lämnad blank så dumpas den i rooten av filherarkin
algorithms =  ["BVH_SR_GENERATE","BVH_SR_FRONT_GENERATE"]
#algorithms =  ["BF","BF_Parallel","BF_SSE","BF_AVX","BF_SSE_Parallel","BF_AVX_Parallel","SAP_Parallel","SAP_SSE","SAP_AVX_Parallel","CGAL","AxisSweep","Grid_2D","Grid_2D_SAP","Grid_2D_Parallel","Grid_2D_SAP_Parallel","Grid_3D","Grid_3D_Parallel","Grid_3D_SAP_Parallel","Tracy","Tracy_Parallel","DBVT D","DBVT F","GPU_Grid","GPU_LBVH","GPU_SAP","KD","BVH_SR_GENERATE"]

# Alla tillgängliga algoritmer
#BF",
#BF_Parallel",
#BF_SSE",
#BF_AVX",
#BF_SSE_Parallel",
#BF_AVX_Parallel",
#SAP",
#SAP_Parallel",
#SAP_SSE",
#SAP_AVX",
#SAP_SSE_Parallel",
#SAP_AVX_Parallel",
#CGAL",
#AxisSweep",
#Grid_2D",
#Grid_2D_SAP",
#Grid_2D_Parallel",
#Grid_2D_SAP_Parallel",
#Grid_3D",
#Grid_3D_SAP",
#Grid_3D_Parallel",
#Grid_3D_SAP_Parallel",
#Tracy",
#Tracy_Parallel",
#DBVT D",
#DBVT F",
#GPU_Grid",
#GPU_LBVH",
#GPU_SAP",
#KD",
#BVH_SR_GENERATE" 
#BVH_SR_FRONT_GENERATE"   
# == Underligan algoritmer finns men är inte garanterade att fungera ==
#BVH_SR_STATIC_MANDATORY"  
#BVH_SR_REFIT_ONLY_FRONT"  
 








additionalProperty = ""
additionaPropertyValues = []

root = scenes_folder


if(output_folder == ""):
	output_folder = scenes_folder +"/_Results/"
else 
    output_folder = output_folder +"/_Results"
    
if (exists(output_folder)):
	rmtree(output_folder) # in case it already existed	
makedirs(output_folder)
pathTmep = ""
for ps in os.listdir(root):
	if(ps[0] != "_"):
		pathTmep = root + '/' + ps + '/'
		break
#mest ineffektiva sättet att beräkna anatalet på, men min hjärna får det inte att gå ihop på npgt emr elegant 
antalTester = 1
# sätt och jag har itne tid att redda ut det bättre sättet.
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
                        if(ob[0] != "_"):
                            pathOB  = pathN +  ob + '/'
                            for alg in algorithms:
                                for t in os.listdir(pathOB) :
                                    if (t[0] != "_"):
                                        antalTester = antalTester+1

nuvarandeTest = 0
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
									nuvarandeTest = nuvarandeTest+1
									pathT         = pathOB + t + '/'
									scenes_folder = pathT + "Recordings_AABBs" + '/'	
									test_name     = "_"+t
									tests_folder  = EJ_generate_jsons(test_name, pathResult, pathalg, scenes_folder, alg, additionalProperty, additionaPropertyValues)
									results_folder = EJ_run_algorithms(tests_folder, broadmark_bin,nuvarandeTest,antalTester)
							proStat = ps.split('_')		
							EJ_calcAverage(pathResult, output_folder, pathalg, proStat[0])
						
EJ_process_results(output_folder)
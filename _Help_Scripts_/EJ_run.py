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


broadmark_bin = "Broadmark_Release_x64.exe" #<= ska peka på broadmark .exe filen
scenes_folder = ''                          #<= ska peka på rooten av hierakin som önskas testas
output_folder = ''                          #<= alternativ att välja var _Results ska sparas, om lämnad blank så dumpas den i rooten av filherarkin
algorithms =  ["BF","Grid_3D_SAP_Parallel","BF_Parallel","BF_SSE","BF_AVX","BF_SSE_Parallel","BF_AVX_Parallel","SAP_Parallel","SAP_SSE","SAP_AVX_Parallel","CGAL","AxisSweep","Grid_2D","Grid_2D_SAP","Grid_2D_Parallel","Grid_2D_SAP_Parallel","Grid_3D","Grid_3D_Parallel","Tracy","Tracy_Parallel","DBVT D","DBVT F","GPU_Grid","GPU_LBVH","GPU_SAP","KD","BVH_SR_GENERATE"]


#Alla tillgängliga algoritmer
# BF",
# BF_Parallel",
# BF_SSE",
# BF_AVX",
# BF_SSE_Parallel",
# BF_AVX_Parallel",
# SAP",
# SAP_Parallel",
# SAP_SSE",
# SAP_AVX",
# SAP_SSE_Parallel",
# SAP_AVX_Parallel",
# CGAL",
# AxisSweep",
# Grid_2D",
# Grid_2D_SAP",
# Grid_2D_Parallel",
# Grid_2D_SAP_Parallel",
# Grid_3D",
# Grid_3D_SAP",
# Grid_3D_Parallel",
# Grid_3D_SAP_Parallel",
# Tracy",
# Tracy_Parallel",
# DBVT D",
# DBVT F",
# GPU_Grid",
# GPU_LBVH",
# GPU_SAP",
# KD",
# BVH_SR_GENERATE" 
# BVH_SR_FRONT_GENERATE"   
#== Underligan algoritmer finns men är inte garanterade att fungera ==
# BVH_SR_STATIC_MANDATORY"  
# BVH_SR_REFIT_ONLY_FRONT"  
 

additionalProperty = ""
additionaPropertyValues = []

root = scenes_folder

if(output_folder == ""):
	output_folder = scenes_folder +"/_Results/"
else: 
    output_folder = output_folder +"/_Results/"
    
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
            if(sim[0] != "_"):
                pathSim = pathPS +  sim + '/'
                for inte in os.listdir(pathSim):
                    pathInte = pathSim + inte + '/'
                    for n in os.listdir(pathInte):
                        if(n[0] != "_"):
                            pathN = pathInte +  n + '/'
                            for ob in os.listdir(pathN):
                                if(ob[0] != "_"):
                                    pathOB  = pathN +  ob + '/'
                                    for alg in algorithms:
                                        for t in os.listdir(pathOB) :
                                            if (t[0] != "_"):
                                                antalTester = antalTester+1

nuvarandeTest = 1

for ps in os.listdir(root):
    if(ps[0] != "_"):
        pathPS = root + '/' + ps + '/'
        for sim in os.listdir(pathPS):
            if(sim[0] != "_"):
                pathSim = pathPS +  sim + '/'
                for inte in os.listdir(pathSim):
                    pathInte = pathSim + inte + '/'
                    for n in os.listdir(pathInte):
                        if(n[0] != "_"):
                            pathN = pathInte +  n + '/'
                            for ob in os.listdir(pathN):
                                if(ob[0] != "_"):
                                    pathOB  = pathN +  ob + '/'
                                    pathRes = pathOB + "_Testning/"
                                    if (exists(pathRes)):
                                        rmtree(pathRes) # in case it already existed
                                    makedirs(pathRes)
                                    for alg in algorithms:
                                        pathalg     = pathRes + alg + '/' # är test_folder
                                        pathResult  = pathalg + "results/"
                                        
                                        makedirs(pathalg)
                                        makedirs(pathResult)
                                        tests_folder   = pathalg
                                        EJ_generate_jsons(pathResult, tests_folder, pathOB, alg, additionalProperty, additionaPropertyValues)
                                        results_folder,nuvarandeTest = EJ_run_algorithms(tests_folder, broadmark_bin,nuvarandeTest,antalTester)
                                        
                                        proStat = ps.split('_')
                                        EJ_calcAverage(pathResult, output_folder, pathalg, proStat[0])
                            


EJ_process_results(output_folder)


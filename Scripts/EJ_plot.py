# -*- coding: utf-8 -*-
from __future__ import print_function, unicode_literals, with_statement, division, absolute_import
from os import listdir, getcwd
from os.path import isfile, isdir, join, basename, getmtime
from EJ_bm_plot import *
from bm_parse import process_results
from os.path import join, isfile


# You can set the results folder to use directly or let the system find the most recent one
results_folder = ''
if results_folder == '':
  #  root = getcwd() + '\\Tests\\'
  #  all_subdirs = [root + d + '\\Results\\' for d in listdir(root)]
  #  latest_subdir = max(all_subdirs, key=getmtime)
  #  results_folder = latest_subdir
	results_folder = "E:/BVH_SR_validering/Resultat"#"C:/Users/Franz/Desktop/TEST_1" #"D:/SKOL_ARBETE/Kurser/ar_3/Vt/Ex_Jobb\BroadMarker/_Resultat/Tests 2020-01-23_18-03-07/Results"#

#plot_lines(results_folder)
#plot_bars(results_folder)
plot_box(results_folder)
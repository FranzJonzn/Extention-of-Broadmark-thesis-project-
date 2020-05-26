# -*- coding: utf-8 -*-
from __future__ import print_function, unicode_literals, with_statement, division, absolute_import
from os import listdir, getcwd
from os.path import isfile, isdir, join, basename, getmtime
from EJ_bm_plot import *
from os.path import join, isfile



results_folder = 'C:/Users/Franz/Desktop/ValideringsTest3/_Results'# <= Ska peka på en _Results fill

plot_lines(results_folder)
plot_bars(results_folder)
plot_box(results_folder)
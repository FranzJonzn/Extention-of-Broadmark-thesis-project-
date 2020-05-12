# -*- coding: utf-8 -*-
from __future__ import print_function, unicode_literals, with_statement, division, absolute_import
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import re
from bm_utils import get_iterable
from os.path import join
from six.moves import zip


def plot_bars(results_folder):

    # Reading the data
    frame = pd.read_pickle(join(results_folder, "multi_index_described_frame.pickle"))
    
    # Fetching scenes, number of objects and algorithms
    scenes     = list(set(frame.index.get_level_values(0)))
    ns         = sorted(list(set(frame.index.get_level_values(2))))
    algorithms = list(set(frame.index.get_level_values(3)))
    ps         = list(set(frame.index.get_level_values(1)))
    
    sns.set(style="whitegrid")
    for scene in scenes:
        for pstatic in ps:
            f, axes = plt.subplots(1, len(ns), figsize=(12, 5), sharex=True, sharey=True)
            for (n, a) in zip(ns, get_iterable(axes)):
                subframe = frame["mean"].transpose()[scene][pstatic][n]#[wanted_algorithms]
                subframe = subframe.sort_values(ascending=False)
                x        = list(subframe.index)
                y        = list(subframe)

                plot = sns.barplot(x=x, y=y, palette="rocket", ax=a)
                plot = plot.set_title(scene + "\n"+re.sub("ps", '', pstatic) + " procent statiska" +"\n" +str(n) + " Objects")
                a.axhline(0, color="k", clip_on=False)
                a.set_ylabel("mean time (s)")
                for tick in a.get_xticklabels():
                    tick.set_rotation(90)
                    
                
        sns.despine(bottom=True)
        plt.tight_layout(h_pad=2)
    plt.show()
		

def plot_box(results_folder):
    # Reading the data
    frame = pd.read_pickle(join(results_folder, "multi_index_frame.pickle"))
    
    # Fetching scenes, number of objects and algorithms
    scenes     = list(set(frame.index.get_level_values(0)))
    ns         = sorted(list(set(frame.index.get_level_values(2))))
    algorithms = list(set(frame.index.get_level_values(3)))
    ps         = list(set(frame.index.get_level_values(1)))
    

    sns.set(style="whitegrid", palette="pastel")
    pal  = sns.cubehelix_palette(len(algorithms), rot=-.5, dark=.3)
    for scene in scenes:
        for p in ps:
            f, axes = plt.subplots(1, len(ns), figsize=(12, 5), sharex=True, sharey=True)
            for (n, a) in zip(ns, get_iterable(axes)):
                subframe = frame.transpose()[scene][p][n]
                plot     = sns.boxplot(data=subframe, ax=a)
                plot     = plot.set_title(scene+ "\n"  +re.sub("ps", '', p) + " procent statiska"+ "\n" + str(n) + " Objects" )
                for tick in a.get_xticklabels():
                    tick.set_rotation(90)

                    
                sns.despine(bottom=True)
                sns.despine(bottom=False)
                plt.tight_layout(h_pad=2)
    plt.show()

def plot_lines(results_folder):
    frame = pd.read_pickle(join(results_folder, "multi_index_described_frame.pickle"))
    scenes = list(set(frame.index.get_level_values(0)))
    algorithms = list(set(frame.index.get_level_values(3)))
    pstatic         = list(set(frame.index.get_level_values(1)))

    for scene in scenes:
        f, axes = plt.subplots(1, len(pstatic), figsize=(12, 5), sharex=False, sharey=True)
        for (p, a) in zip(pstatic, get_iterable(axes)):
            subframe                = frame["mean"].transpose()[scene]#[wanted_algorithms]
            subframe                = subframe.reset_index()
            subframe.rename(columns = {'level_2':'Algorithm','level_0':'ps','level_1':'N (10³)',  'mean':'mean time'}, inplace=True)
            subframe['N (10³)']     = subframe['N (10³)'] / 1000
      
            a.axhline(0, color="k", clip_on=True)
           
            if len(algorithms) <= 6:
                plot = sns.lineplot(x='N (10³)', y='mean time', hue='Algorithm', style="Algorithm",  palette="rocket", ax=a, markers=True, data=subframe)
            else:
                plot = sns.lineplot(x='N (10³)', y='mean time', hue='Algorithm', ax=a, markers=True, data=subframe)
            
            plot = plot.set_title(scene+ " \n" +re.sub("ps", '', p) + " procent statiska" )
            a.set_ylabel("mean time (s)")
            for tick in a.get_xticklabels():
                tick.set_rotation(90)
                
            
            sns.despine(bottom=True)
            plt.tight_layout(h_pad=2)
    plt.show()

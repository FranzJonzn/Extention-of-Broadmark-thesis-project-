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
    ps           = sorted(list(set(frame.index.get_level_values(1))))
    
    #Soretera listan i storleks ordninge, annars så sätteer den 100 efter noll
    if len(ps) >= 2:
        psW   = ps[1]
        for x in range(1, len(ps)-1):
            ps[x] = ps[x+1]
        ps[ len(ps)-1]   = psW
    
    
    
    NumberOfTest = len(scenes)*len(ns)*len(ps)
    testVisade = 0

    sns.set(style="whitegrid")

    for n in ns:
        for scene in scenes:
            f, axes = plt.subplots(1, len(ps), figsize=(len(algorithms), 10), sharex=True, sharey=True)
            for (pstatic, a) in zip(ps, get_iterable(axes)):
                subframe   = frame["mean"].transpose()[scene][pstatic][n]#[wanted_algorithms]
                subframe   = subframe.sort_values(ascending=False)
                x          = list(subframe.index)
                y          = list(subframe)
                testVisade = testVisade+1
        
                if len(algorithms) <= 6:
                    plot    = sns.barplot(x=x, y=y, palette="rocket", ax=a)
                else:
                    plot    = sns.barplot(x=x, y=y, ax=a)

                plot       = plot.set_title(scene + "\n"+re.sub("ps", '', pstatic) + " procent statiska" +"\n" +str(n) + " Objects")
                a.axhline(0, color="k", clip_on=True)
                a.set_ylabel("mean time (s)")
      
                for tick in a.get_xticklabels():
                    tick.set_rotation(90)
                    
                
            sns.despine(bottom=True)
            plt.tight_layout(h_pad=2)
            print("[Laddar] "+str(testVisade)+"/" + str(NumberOfTest))
        print("[Visa]")
        plt.show()
		

def plot_box(results_folder):
    # Reading the data
    frame = pd.read_pickle(join(results_folder, "multi_index_frame.pickle"))
    
    # Fetching scenes, number of objects and algorithms
    scenes     = list(set(frame.index.get_level_values(0)))
    ns         = sorted(list(set(frame.index.get_level_values(2))))
    algorithms = list(set(frame.index.get_level_values(3)))
    ps           = sorted(list(set(frame.index.get_level_values(1))))
    
    #Soretera listan i storleks ordninge, annars så sätteer den 100 efter noll
    if len(ps) >= 2:
        psW   = ps[1]
        for x in range(1, len(ps)-1):
            ps[x] = ps[x+1]
        ps[ len(ps)-1]   = psW
    NumberOfTest = len(scenes)*len(ns)*len(ps)
    testVisade   = 0
    sns.set(style="whitegrid", palette="pastel")
    pal  = sns.cubehelix_palette(len(algorithms), rot=-.5, dark=.3)
    for n in ns:
        for s in scenes:
            f, axes = plt.subplots(1, len(ps), figsize=(len(algorithms), 5), sharex=True, sharey=True)
            for (p, a) in zip(ps, get_iterable(axes)):
                subframe   = frame.transpose()[s][p][n]
                plot       = sns.violinplot(data=subframe, ax=a, palette=pal)#sns.boxplot(data=subframe, ax=a)
                testVisade = testVisade+1
                plot       = plot.set_title(s+ "\n"  +re.sub("ps", '', p) + " procent statiska"+ "\n" + str(n) + " Objects" )
                for tick in a.get_xticklabels():
                    tick.set_rotation(90)
            
            sns.despine(bottom=False)
            plt.tight_layout(h_pad=2)
            print("[Laddar] "+str(testVisade)+"/" + str(NumberOfTest))
        print("[Visa]")
        plt.show()

def plot_lines(results_folder):
    frame        = pd.read_pickle(join(results_folder, "multi_index_described_frame.pickle"))
    scenes       = list(set(frame.index.get_level_values(0)))
    algorithms   = list(set(frame.index.get_level_values(3)))
    ps           = sorted(list(set(frame.index.get_level_values(1))))
    
    #Soretera listan i storleks ordninge, annars så sätteer den 100 efter noll
    if len(ps) >= 2:
        psW   = ps[1]
        for x in range(1, len(ps)-1):
            ps[x] = ps[x+1]
        ps[ len(ps)-1]   = psW

    NumberOfTest = len(scenes)*len(ps)
    testVisade = 0

    for s in scenes:
        f, axes = plt.subplots(1, len(ps), figsize=(15, 7), sharex=False, sharey=True)
        
        curent = 0
        for (p, a) in zip(ps, get_iterable(axes)):
            subframe                = frame["mean"].transpose()[s][p]#[wanted_algorithms]
            subframe                = subframe.reset_index()
            subframe.rename(columns = {'level_1':'Algorithm','level_0':'N (10³)',  'mean':'mean time'}, inplace=True)
            subframe['N (10³)']     = subframe['N (10³)'] / 1000
            a.axhline(0, color="k", clip_on=True)
            testVisade = testVisade+1

            if len(algorithms) <= 6:
                plot = sns.lineplot(x='N (10³)', y='mean time', hue='Algorithm', style="Algorithm",  palette="rocket", ax=a, markers=True, data=subframe)
            else:
                plot = sns.lineplot(x='N (10³)', y='mean time', hue='Algorithm', ax=a, markers=True, data=subframe)
            plot = plot.set_title(s+ " \n" +re.sub("ps", '', p) + " procent statiska" )
 
            a.set_ylabel("mean time (s)")
            
            curent = curent+1
            if len(ps) != curent:
                a.legend().set_visible(False)#legend('hide')

            for tick in a.get_xticklabels():
                tick.set_rotation(90)
                
            
            sns.despine(bottom=False)
            plt.tight_layout(h_pad=2)
            print("[Laddar] "+str(testVisade)+"/" + str(NumberOfTest))
        print("[Visa]")
        plt.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.)
        plt.show()

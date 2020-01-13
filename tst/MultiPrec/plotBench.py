import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter
from matplotlib.ticker import FixedLocator
from matplotlib.ticker import NullLocator
from matplotlib.ticker import MultipleLocator


def plotBench(benchs,basename,number=-1):
    extent=str(number)
    if number==-1:
        number=len(benchs)
        extent=""
    for bench in benchs:
        bench.setMax()
#    benchs=[ item.setMax() for item in inputBenchs]
    print(benchs)
    benchs.sort(key=lambda item:item.max,reverse=True)
    plt.rc('text', usetex=True)
    plt.figure(figsize=(7,5.5), dpi=180)
    plt.subplot(111)
    new_colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728',
              '#9467bd', '#8c564b', '#e377c2', '#7f7f7f',
              '#bcbd22', '#17becf']
    markers=['o','s','^','D','*','+']
    j=0
    for bench in benchs:
        print("name=",bench.name)
        if j>=len(benchs)-number:
            if bench.name.find("parmap")!=-1:
                plt.semilogx(bench.sizes,bench.perfs,color=new_colors[j],linewidth=2.0, linestyle="--",marker="o",fillstyle="full",markeredgewidth=2,markerfacecolor="white",markersize=6,label=str(bench.name))
            else:
                plt.semilogx(bench.sizes,bench.perfs,color=new_colors[j],linewidth=2.0, linestyle="-",marker="o",fillstyle="full",markeredgewidth=2,markerfacecolor="white",markersize=6,label=str(bench.name))
        j+=1
    plt.tick_params(axis='both', which='major', labelsize=20)

    pmax=0.0
    for bench in benchs:
        if pmax<max(bench.perfs):pmax=max(bench.perfs)
    ymax=1.1*pmax
    
    plt.ylim(0,ymax)
    plt.xlim(8,512)
    xt=[4,8,16,32,64,128,256,512]
    ax = plt.gca()
    ax.xaxis.set_major_locator(FixedLocator(xt))
    ax.xaxis.set_major_formatter(ScalarFormatter())
    #ax.yaxis.set_major_formatter(ScalarFormatter())
#    ml = MultipleLocator(5)
#    ax.yaxis.set_major_locator(ml)
#    ax.yaxis.set_minor_locator(ml)
#    ax.yaxis.set_minor_locator(minorLocator)

    ax.xaxis.set_minor_locator(NullLocator())
    last=benchs[len(benchs)-number]

    index=benchs[0].perfs.index(max(benchs[0].perfs))
#    index=len(last.sizes)//2
    bbox_props = dict(boxstyle="round,pad=0.1", fc="w", ec="1.0", alpha=1.0)
#    ax.annotate(str(last.perfs[index]),xy=(last.sizes[index],last.perfs[index]), xycoords='data',xytext=(last.sizes[index],last.perfs[index]),textcoords='data',rotation=30,size=13,bbox=bbox_props)
    ax.annotate(str(round(last.perfs[index],1))+" GFlops",xy=(last.sizes[index],last.perfs[index]), xycoords='data',xytext=(last.sizes[index]*0.5,5+last.perfs[index]),textcoords='data',size=13,bbox=bbox_props,arrowprops=dict(arrowstyle="->",facecolor='black'))
    index=len(last.sizes)-1
    ax.annotate(str(round(last.perfs[index],1))+" GFlops",xy=(last.sizes[index],last.perfs[index]), xycoords='data',xytext=(last.sizes[index]*0.5,5+last.perfs[index]),textcoords='data',size=13,bbox=bbox_props,arrowprops=dict(arrowstyle="->",facecolor='black'))
    plt.grid(True,"both")
    plt.legend(labelspacing=0.2,fontsize=12)
    plt.xlabel(r"$\mbox{Systems size :}~ n_x~~(\mbox{Nb of systems:}~n_y=n_x^2)$",fontsize=18)
    plt.ylabel(r"$\mbox{GFlops}$",fontsize=18)

    plt.tight_layout()

    fig=plt.gcf()
    name=basename+extent
    fig.savefig(name+".pdf", format='pdf', dpi=1200)



        

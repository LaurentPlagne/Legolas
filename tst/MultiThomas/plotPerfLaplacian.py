import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter
from matplotlib.ticker import FixedLocator
from matplotlib.ticker import NullLocator

class Bench:

    def __init__(self,algo,ps,par):
        self.algo=algo
        self.ps=int(ps)
        self.par=par
        self.sizes=[]
        self.perfs=[]
        self.max=0.0


files=os.listdir('.')
plotFiles=[f for f in files if f.find(".dat")!=-1]
plotFiles=[f for f in plotFiles if f.find("Laplacian")!=-1]           

bench=[]
for f in plotFiles:
    print("f=",f)
    val=f.split("_")
    item=Bench(val[0],val[1],val[2].replace(".dat","")=="parallel")
    print(item.algo)
    print(item.ps)
    print(item.par)
    file=open(f,"r")
    of=file.read()
    file.close()
    lines=of.split("\n")
    for i in range(len(lines)-1):
        words=lines[i].split(" ")
        print("line",lines[i])
        print("words[0]",words[0])
        print("words[1]",words[1])
        item.sizes.append(int(words[0]))
        item.perfs.append(float(words[1]))
        
    item.max=item.perfs[len(item.perfs)//2]    
    bench.append(item)

bench.sort(key=lambda item:item.max,reverse=True)

for i in bench:
    print(i.algo)
    for j in range(len(i.sizes)):
        print(i.sizes[j])
        print(i.perfs[j])


    
plt.rc('text', usetex=True)
plt.figure(figsize=(5,5.5), dpi=180)
plt.subplot(111)
#legstring="$\mbox{\tt{Array<}}$"
#leg=r"$\mbox{\tt{Array<}}$"


new_colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728',
              '#9467bd', '#8c564b', '#e377c2', '#7f7f7f',
              '#bcbd22', '#17becf']
markers=['o','s','^','D','*','+']
j=0
for item in bench:
#    legstring="$\mbox{"+item.algo+", PS= "+str(item.ps)+", "
    legstring="$\mbox{PS= "+str(item.ps)+", "
    if item.par:
        legstring+="parmap"
    else:
        legstring+="map"
    legstring+="}$"
    plt.semilogx(item.sizes,item.perfs,color=new_colors[j],linewidth=2.0, linestyle="-",marker="o",fillstyle="full",markeredgewidth=2,markerfacecolor="white",markersize=6,label=legstring)
#    plt.semilogx(item.sizes,item.perfs,color=new_colors[j],linewidth=2.0, linestyle="-",marker="o",fillstyle="full",markeredgewidth=2,markerfacecolor="white",markersize=6,label=legstring)
#    plt.semilogx(item.sizes,item.perfs,color=new_colors[j],linewidth=2.0, linestyle="-",label=legstring)
    j+=1
    plt.rc('font', weight='bold')
plt.tick_params(axis='both', which='major', labelsize=20)
plt.ylim(0,120)
plt.xlim(8,512)
xt=[4,8,16,32,64,128,256,512]
ax = plt.gca()
ax.xaxis.set_major_locator(FixedLocator(xt))
ax.xaxis.set_major_formatter(ScalarFormatter())
#ax.yaxis.set_major_formatter(ScalarFormatter())
ax.xaxis.set_minor_locator(NullLocator())
plt.grid(True,"both")
plt.legend(labelspacing=0.2,fontsize=12)
plt.xlabel(r"$\mbox{Systems size :}~ n_x~~(n_y=n_x^2)$",fontsize=18)
plt.ylabel(r"$\mbox{GFlops}$",fontsize=18)

plt.tight_layout()

fig=plt.gcf()
fig.savefig('Laplacian.svg', format='svg', dpi=1200)
fig.savefig('Laplacian.pdf', format='pdf', dpi=1200)

#plt.show()
    
    

import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter
from matplotlib.ticker import FixedLocator
from matplotlib.ticker import NullLocator
import plotBench as pb

basename="Thomas"
#basename="Laplacian"
#basename="ThomasLDLSolver"
findname=basename+"_"
files=os.listdir('.')
plotFiles=[f for f in files if f.find(".dat")!=-1]
plotFiles=[f for f in plotFiles if f.find(findname)!=-1]

benchs=[]
for f in plotFiles:
    print("f=",f)
    val=f.split("_")
    if len(val)==4:
        name=""
        if val[2]=="parallel":
            name+="parmap"
        else:
            name+="map"
        name+=", $\mbox{P= "+str(val[1])
        name+="}$"
        nthread=val[3].replace(".dat","")
        name+=", $\mbox{thread= "+str(nthread)
        name+="}$"
        item=pb.Bench(name)
        print(item.name)
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
        benchs.append(item)

    print("len(benchs)=",len(benchs))
basename+="_SpUp"
for i in range(1,len(benchs)+1):
    pb.plotBench(benchs,basename,i)



import re;
import os;
import sys;
import io;
import pathlib

import matplotlib
from matplotlib import pyplot as plt
import pandas as pd
import numpy
import tkinter as tk
from tkinter import filedialog

matplotlib.use("pgf")
matplotlib.rcParams.update({
    "pgf.texsystem": "pdflatex",
    'font.family': 'serif',
    'text.usetex': True,
    'pgf.rcfonts': False,
})

Leerlauf = "TestTemp"

realNames = {
    "TestTemp" : "Leertest",
    "pkruS"    : "Pur",
    "pkruSL"   : "Schleife",
    "pkruWB"   : "Schreibzugriff zuvor",
    "pkruW"    : "Schreibzugriff danach",
    "pkruBTW"  : "Schreibzugriff zuvor und danach",
    "pkruWL"   : "Schreibzugriff Schleife",

    "sysS"    : "Pur",
    "sysSL"   : "Schleife",
    "sysWB"   : "Zuvor ohne TLB miss",
    "sysWBMIS": "Zuvor mit TLB miss",
    "sysW"    : "Schreibzugriff danach",
    "sysBTW"  : "Schreibzugriff zuvor und danach",
    "sysWL"   : "Schreibzugriff Schleife"
}

countPerTimeLib = {
    
}

colour = {
    "TestTemp" : "black",
    "pkruS"    : "lime",
    "pkruSL"   : "orange",
    "pkruWB"   : "gold",
    "pkruW"    : "royalblue",
    "pkruBTW"  : "royalblue",
    "pkruWL"   : "orange",

    "sysS"    : "lime",
    "sysSL"   : "orange",
    "sysWB"   : "gold",
    "sysWBMIS": "orangered",
    "sysW"    : "royalblue",
    "sysBTW"  : "royalblue",
    "sysWL"   : "orange"
}

dataframes = {

}

drawBarGraph = True

def doplotthings(plotname, *testNames):
    leerlaufDf = dataframes[Leerlauf]
    leerlaufMean = numpy.nanmean(leerlaufDf)

    barTestNames = []
    barTestMeanString = []
    barTestMeanValues = []
    barTestColours = []

    for testName in testNames:
        currentdf = dataframes[testName]
        currentdf = currentdf.iloc[1:]
        currentColour = colour[testName]
        currentName = realNames[testName]
        

        if "L" not in testName:
            currentdf = currentdf.sub(leerlaufMean)
            # currentdf = currentdf.rolling(3).mean()
        else:
            countPerTime = countPerTimeLib[testName]
            leerlaufPerTry = leerlaufMean / int(countPerTime)
            currentdf = currentdf.sub(leerlaufPerTry)

        mean = numpy.nanmean(currentdf)
        barTestNames.append(currentName)
        barTestMeanValues.append(mean)
        barTestMeanString.append(str(round(mean, 1)))
        barTestColours.append(currentColour)
        print(currentName + " durschnittlich: " + str(round(mean, 1)))
        currentdf = currentdf.iloc[::25, :]
        if not drawBarGraph:
            plt.plot(currentdf, label=(currentName + " diameter: " + str(round(mean, 1))), linestyle='None', marker='o', markersize=1, color=currentColour)
            meanline = plt.axhline(y=mean)
            meanline.set_color(currentColour)
            meanline.set_linestyle('dashed')

    if drawBarGraph:
        x = numpy.arange(len(barTestMeanString))
        plt.bar(barTestMeanString, barTestMeanValues, color=barTestColours, width=0.4, label=barTestNames)
        plt.xticks(x, barTestMeanString)
        plt.xlabel("Testdurchlauf")
    else:
        plt.xlabel("Testdurchlauf Nr.")   
    
    plt.ylabel("TSC Cycles")
    plt.legend(loc="center right")     
    plt.savefig(save_path + "/" + plotname + ".pgf")
    plt.savefig(save_path + "/" + plotname + ".pdf", format="pdf", bbox_inches="tight")
    plt.clf()


root = tk.Tk()
root.withdraw()

file_path = filedialog.askopenfilename()

save_path = str(pathlib.Path(file_path).parent)

with open(file_path, 'r') as file:
    results = file.read()

regex = re.compile(r'(count per time = (?P<count_per_time>\d*).*(\r\n|\r|\n))?(?P<identifier>\w*).*--RegexStartCSVMarker--(?P<csvFile>.*)--RegexEndCSVMarker--', re.MULTILINE)

with pd.ExcelWriter(save_path + "/" + "result.xlsx") as excelWriter:
    for x in regex.finditer(results):
        identifier = x.group("identifier")
        countPerTime = x.group("count_per_time")
        csvFile = x.group("csvFile")
        print(identifier)
        df = pd.read_csv(io.StringIO(csvFile), sep=',', lineterminator=';')
        if(countPerTime is not None):
            df = df.div(int(countPerTime))
            countPerTimeLib[identifier] = countPerTime

        mean = numpy.nanmean(df)
        df.drop(df[df['durationTSC'] > (mean + 1000)].index, inplace= True)
        dataframes[identifier] = df
        
        df.to_excel(excelWriter, sheet_name=identifier)


doplotthings("SystemCall", "sysWBMIS", "sysW", "sysWB", "sysSL", "sysS")
doplotthings("SystemCall Loop", "sysWL", "sysBTW")

doplotthings("PKRU Simple", "pkruS", "pkruSL")
doplotthings("PKRU Write", "pkruWB", "pkruW")
doplotthings("PKRU Write Loop", "pkruWL", "pkruBTW")



    
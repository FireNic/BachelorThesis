import re;
import os;
import sys;
import io;
import pathlib

import matplotlib
from matplotlib import pyplot as plt
import pandas as pd
import tkinter as tk
from tkinter import filedialog

matplotlib.use("pgf")
matplotlib.rcParams.update({
    "pgf.texsystem": "pdflatex",
    'font.family': 'serif',
    'text.usetex': True,
    'pgf.rcfonts': False,
})

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

        df = pd.read_csv(io.StringIO(csvFile), sep=',', lineterminator=';')

        if(countPerTime is not None):
            df = df.div(int(countPerTime))

        df.plot(y=['durationTSC'], ylim=(0, None))
        df.to_excel(excelWriter, sheet_name=identifier)
        plt.savefig(save_path + "/" + identifier + ".pgf")
    
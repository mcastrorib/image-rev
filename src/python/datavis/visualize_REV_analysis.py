import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

folders = sorted(os.listdir('revs'))
print("current saved REV analysis:")
folderID = 0
for folder in folders:
    print(folderID, '>', folder)
    folderID += 1

option = int(input('choose the folder: '))

if(option < len(folders)):
    REV_file = os.path.join('revs', folders[option], 'REV_analysis.csv')
    df = pd.read_csv(REV_file)
    REV_sizes = df["REV_SIZE"].to_numpy()
    REV_phis = df["MEAN_REV_POROSITY"].to_numpy()

    fig, axs = plt.subplots(1,1, figsize=[6,5])
    axs.plot(REV_sizes, REV_phis, 'o-', ms=4)
    axs.set_ylim([0, 1])
    axs.set_xlim([0, 1.01*REV_sizes.max()])
    axs.set_xlabel('REV size (voxels)')
    axs.set_ylabel('Porosity')
    
    plt.tight_layout()
    plt.show()
    save_opt = "x"
    while(save_opt not in ["N","n"]):
        save_opt = input('Save figure? (Y/N): ')
        if(save_opt == "Y" or save_opt == "y"):
            fig, axs = plt.subplots(1,1, figsize=[6,5])
            REV_figpath = os.path.join('revs', folders[option], 'REV_analysis.svg')
            axs.plot(REV_sizes, REV_phis, 'o-', ms=4)
            axs.set_ylim([0, 1])
            axs.set_xlim([0, 1.01*REV_sizes.max()])
            axs.set_xlabel('REV size (voxels)')
            axs.set_ylabel('Porosity')
            plt.savefig(REV_figpath, format='svg')
            save_opt = "N"
else:
    print('invalid option')
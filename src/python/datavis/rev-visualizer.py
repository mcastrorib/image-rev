import os
import sys
import numpy as np
import pandas as pd
import matplotlib
import matplotlib.pyplot as plt

def main(dirpath):
	REV_file = os.path.join(dirpath, 'REV_analysis.csv')
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
	return

if __name__ == "__main__":
    if(len(sys.argv) > 1):
	    main(sys.argv[1])

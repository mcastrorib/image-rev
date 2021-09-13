import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def get_folder_for_visualization(db_dir):
    folders = sorted(os.listdir(db_dir))
    n_folders = len(folders)
    print("current saved NMR simulations:")
    folderID = 0
    for folder in folders:
        print(folderID, '>', folder)
        folderID += 1

    # Read and process user input
    user_inputs = input('choose the folder: ').split(' ')

    selected_folders = []
    valid_inputs = []   
    invalid_inputs = []
    for user_input in user_inputs:
        try:
            fId = int(user_input)
            if not (fId < 0 or fId > n_folders):
                if not (fId in valid_inputs):
                    print(fId, '>', folders[fId])
                    valid_inputs.append(fId)
                    selected_folders.append(folders[fId])
        except:
            invalid_inputs.append(user_input)

    return selected_folders

def get_folders_labels(folders):
    labels = []
    print('')
    for folder in folders:
        new_label = input('>> Set label to folder ' + folder + ': ')
        labels.append(new_label)
    print('')
    return labels

def main():
    folders = get_folder_for_visualization('revs')
    labels = get_folders_labels(folders)

    fig, axs = plt.subplots(1,1, figsize=[8,6])
    axs.set_title('REV analysis')
    max_sizes = []
    for folderId in range(len(folders)):

        REV_file = os.path.join('revs', folders[folderId], 'REV_analysis.csv')
        df = pd.read_csv(REV_file)
        REV_sizes = df["REV_SIZE"].to_numpy()
        REV_phis = df["MEAN_REV_POROSITY"].to_numpy()
        max_sizes.append(REV_sizes.max())

        axs.plot(REV_sizes, REV_phis, 'o-', ms=2.5, label=labels[folderId])
    
    axs.legend(loc="best")
    axs.set_ylim([0,1])
    axs.set_xlim([0, 1.01*max(max_sizes)])
    axs.set_xlabel('REV size (voxels)')
    axs.set_ylabel('Porosity')
    
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main() 
from mpi4py import MPI
import cv2
import tkinter as tk
from tkinter import filedialog

comm = MPI.COMM_WORLD
worldRank = comm.Get_rank()
worldSize = comm.Get_size()
fpath = None
if worldRank == 0:
    tkroot = tk.Tk()
    tkroot.withdraw()
    fpath = filedialog.askopenfilename()
else:
    fpath = None
fpath = comm.bcast(fpath, root = 0)

source = cv2.imread(fpath)

def medianfilter(src):
    members=[src[0,0]]*9
    for a in range(1,src.shape[0]-1):
        for b in range(1,src.shape[1]-1):
            members[0] = src[a-1,b-1]
            members[1] = src[a,b-1]
            members[2] = src[a+1,b-1]
            members[3] = src[a-1,b]
            members[4] = src[a,b]
            members[5] = src[a+1,b]
            members[6] = src[a-1,b+1]
            members[7] = src[a,b+1]
            members[8] = src[a+1,b+1]

            members.sort()
            temp = src[:]
            temp[a,b]=members[4]
    return temp

def main():
    subset = None
    result = None
    time = 0
    if worldSize != 3:
        return
    if worldRank == 0:
        time -= MPI.Wtime()
    (B,G,R) = cv2.split(source)
    bgr = [B,G,R]
    subset = comm.scatter(bgr, root = 0)
    subset = medianfilter(subset)
    comm.barrier()
    data = comm.gather(subset, root = 0)
    if worldRank == 0:
        result = cv2.merge(data)
        time += MPI.Wtime()
        cv2.imshow("Source",source)
        cv2.imshow("Result",result)
        print('Parallel time to denoise image:', time)
        savepath = filedialog.asksaveasfilename(title = "Save result image")
        if savepath != None:
            cv2.imwrite(savepath, result)
        else:
            print('Parallel time to denoise image:', time)
        cv2.waitKey(0)
    

if __name__ == "__main__":
    main()

import numpy as np
import matplotlib.pyplot as plt

class Data :
    def __init__(self, M):
        self.M = M
        filename = "M" + str(self.M) + "P" + str(1) + ".txt"
        self.dataP1 = np.loadtxt(filename)
        filename = "M" + str(self.M) + "P" + str(2) + ".txt"
        self.dataP2 = np.loadtxt(filename)
        filename = "M" + str(self.M) + "P" + str(4) + ".txt"
        self.dataP4 = np.loadtxt(filename)
        filename = "M" + str(self.M) + "P" + str(8) + ".txt"
        self.dataP8 = np.loadtxt(filename)

        meanP1 = np.mean(self.dataP1)
        meanP2 = np.mean(self.dataP2)
        meanP4 = np.mean(self.dataP4)
        meanP8 = np.mean(self.dataP8)

        self.graphicsData = np.array([meanP1, meanP2, meanP4, meanP8])

    def showGraphics(self):
        x = np.array([1, 2, 4, 8])
        plt.plot(x, self.graphicsData)
        plt.title(str(self.M) + "x" + str(self.M) + " size matrix")
        plt.show()

def __main__():

    M8 = Data(8)
    M64 = Data(64)
    M256 = Data(256)
    M512 = Data(512)
    M1024 = Data(1024)

    M8.showGraphics()
    M64.showGraphics()
    M256.showGraphics()
    M512.showGraphics()
    M1024.showGraphics()

if __name__ == '__main__' :
    __main__()

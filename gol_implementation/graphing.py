import math 
import matplotlib.pyplot as plt
import numpy as np

### times are in miliseconds as of 11-28 output
yproc1 = [563,2276,9168,37020,148715,591909]
yproc4 = [196.211918,627.864927,2354.994699,9355.517531,37710.128990,152349.691746]
yproc16 = [85.906608,194.270004,658.120969,2494.560939,9911.096148,40756.708791]
yproc64 = [122.133957, 146.707054,256.279779,736.280016,2565.692906,10047.984399]


#use this if you want to convert all times from miliseconds to seconds
# divisor = 1000
# yproc1 = [x/divisor for x in yproc1]
# yproc4 = [x/divisor for x in yproc4]
# yproc16 = [x/divisor for x in yproc16]
# yproc64 = [x/divisor for x in yproc64]




x = [256, 512, 1024, 2048, 4096, 8192]
#x = [0,1,2,3,4,5]

plt.yscale("log")
plt.plot(x,yproc1)
plt.plot(x,yproc4)
plt.plot(x,yproc16)
plt.plot(x,yproc64)


legenditems = []
legenditems.append("Serial Implementation")
legenditems.append("Proc size 4")
legenditems.append("Proc size 16")
legenditems.append("Proc size 64")
plt.legend(legenditems)
plt.xlabel('Dimensions of Board')
plt.ylabel('time to compete')

plt.title('Time to run 1000 iterations of game of life')
plt.show()

import math 
import matplotlib.pyplot as plt
import numpy as np

### times are in miliseconds as of 11-28 output
yproc1 = [563,2276,9168,37020,148715,591909]
yproc4 = [196.211918,627.864927,2354.994699,9355.517531,37710.128990,152349.691746]
yproc16 = [85.906608,194.270004,658.120969,2494.560939,9911.096148,40756.708791]
yproc64 = [122.133957, 146.707054,256.279779,736.280016,2565.692906,10047.984399]
yshm = [163.540762,635.596914, 2568.209545, 10324.706500, 41396.629173, 165584.079762]


#use this if you want to convert all times from miliseconds to seconds
# divisor = 1000
# yproc1 = [x/divisor for x in yproc1]
# yproc4 = [x/divisor for x in yproc4]
# yproc16 = [x/divisor for x in yproc16]
# yproc64 = [x/divisor for x in yproc64]




x = [256, 512, 1024, 2048, 4096, 8192]
#x = [0,1,2,3,4,5]

plt.yscale("log")
plt.plot(x,yproc1, 'b')
plt.plot(x,yproc4, 'r-')
plt.plot(x,yproc16, 'g')
plt.plot(x,yproc64, 'purple')
plt.plot(x,yshm, 'r--')


legenditems = []
legenditems.append("Serial")
legenditems.append("4 processes")
legenditems.append("16 processes")
legenditems.append("64 processes")
legenditems.append("4 processes - shared memory")
plt.legend(legenditems)
plt.xlabel('Board Dimension')
plt.ylabel('Time to complete (ms)')

plt.title('Game of Life - 1000 iterations')
plt.show()


yproc1io = [3747, 10173, 35492, 136421, 538317, 2146406]
yproc4io = [1438.013100, 3287.531851, 8809.085941, 30716.137408, 125540.299769, 530003.057539]
yproc16io = [1277.468515, 2817.932449, 7884.520013, 30025.421361, 123334.646814, 430094.012784]
yproc64io = [1376.079770, 3176.006396, 7569.168576, 25422.613087, 97299.925169, 488672.805999]
ysharedio = [1585.989569, 9676.838933, 16332.876709, 33970.637465, 128614.502023, 499542.104651]

#plt.xscale("log")
plt.yscale("log")
plt.plot(x,yproc1io, 'b')
plt.plot(x,yproc4io, 'r-')
plt.plot(x,yproc16io, 'g')
plt.plot(x,yproc64io, 'purple')
plt.plot(x,ysharedio, 'r--')


legenditems = []
legenditems.append("Serial")
legenditems.append("4 processes")
legenditems.append("16 processes")
legenditems.append("64 processes")
legenditems.append("4 processes - shared memory")

plt.legend(legenditems)
plt.xlabel('Board Dimension')
plt.ylabel('Time to complete (ms)')


plt.title('Game of Life - 1000 iterations - IO')
plt.show()



yproc1io = [y*1 for y in yproc1io]
yproc4io = [y*4 for y in yproc4io]
yproc16io = [y*16 for y in yproc16io]
yproc64io = [y*64 for y in yproc64io] 
ysharedio = [y*4 for y in ysharedio]

#plt.xscale("log")
plt.yscale("log")
plt.plot(x,yproc1io, 'b')
plt.plot(x,yproc4io, 'r-')
plt.plot(x,yproc16io, 'g')
plt.plot(x,yproc64io, 'purple')
plt.plot(x,ysharedio, 'r--')


legenditems = []
legenditems.append("Serial Implementation")
legenditems.append("4 processes")
legenditems.append("16 processes")
legenditems.append("64 processes")
legenditems.append("4 processes - shared memory")

plt.legend(legenditems)
plt.xlabel('Board Dimension')
plt.ylabel('Time to complete (ms)')


plt.title('1000 iterations of game of life with IO normalized')
plt.show()


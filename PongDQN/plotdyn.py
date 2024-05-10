#For plotting train/test log (dynamic)
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import time
# filepath="training/models/historylog_model3.csv"
filepath="training/models/testlog_model3.csv"
# filepath="training/model2/testlog_newmodelnewr1_3.csv"
data=pd.read_csv(filepath)
data.dropna(inplace=True)
data=data.iloc[:,:]
print(data.describe())
plt.ion()
hl, =plt.plot(data["episode"],data["netreward"],label="net reward")
# print(data.groupby(np.arange(len(data))//5).mean())
data2=data.groupby(np.arange(len(data))//20).mean()
# hl, =plt.plot([],[])
hl2, =plt.plot(data2["episode"],data2["netreward"],label="mean reward")
plt.legend()
plt.ylabel("Net reward")
plt.xlabel("episode")
# plt.show()
def update(hl,hl2,data,data2):

    # print(data.describe())
    plt.clf()
    plt.plot(data["episode"],data["netreward"],label="net reward")
    # plt.draw()
    plt.plot(data2["episode"],data2["netreward"],label="mean reward")
    plt.legend()
    plt.ylabel("Net reward")
    plt.xlabel("episode")
    plt.draw()
    
    plt.pause(1)
    
plt.show(block=False)
print("loop")
while len(plt.get_fignums()):
    # if int(time.time()) % 5:
        # print("update")
        data=pd.read_csv(filepath)
        data.dropna(inplace=True)
        data=data.iloc[:,:]
        data2=data.groupby(np.arange(len(data))//20).mean()
        update(hl,hl2,data,data2)
# plt.plot(data2["episode"],data2["netreward"],label="mean reward")

# print(data)
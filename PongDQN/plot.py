#For plotting train/test log (static)
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
data=pd.read_csv("training/models/testlog_model2.csv")
data.dropna(inplace=True)
data=data.iloc[:,:]
print(data.describe())
plt.plot(data["episode"],data["netreward"],label="net reward")
# print(data.groupby(np.arange(len(data))//5).mean())
data2=data.groupby(np.arange(len(data))//20).mean()
plt.plot(data2["episode"],data2["netreward"],label="mean reward")
plt.legend()
plt.ylabel("Net reward")
plt.xlabel("episode")
plt.show()
# print(data)

import matplotlib.pyplot as plt
import numpy as np
from numpy.core.fromnumeric import transpose
from numpy.random.mtrand import shuffle
from mlp_lib import MultiLayerPerceptron

# np.random.seed(1)

def read_data_set():
    data_1 = []
    data_2 = []
    data_3 = []
    with open("iris.data") as f:
        line = f.read().split("\n")
        for s in line:
            if s == "":
                continue

            s_data = s.split(",")
            
            data = []
            for d in s_data[:-1]:
                data.append(float(d))

            data = np.reshape(data,(4,1))

            name = s_data[-1]

            if  name == "Iris-setosa":
                d_type = np.zeros((3,1))
                d_type[0] = 1
                data_1.append([data, d_type])
            elif name == "Iris-versicolor":
                d_type = np.zeros((3,1))
                d_type[1] = 1
                data_2.append([data, d_type])
            elif name == "Iris-virginica":
                d_type = np.zeros((3,1))
                d_type[2] = 1
                data_3.append([data, d_type])

    data_set = [data_1, data_2, data_3]
    data_set = scale_inputs(data_set, [0.1, 0.9])
    t_size = 40

    train_set = []
    test_set = []
    for d in data_set:
        shuffle(d)
        train_set = train_set + d[0:t_size]
        test_set = test_set + d[t_size:]

    return train_set , test_set

def scale_inputs(data,scale_range):
    x_min = min(scale_range)
    x_max = max(scale_range)

    max_l = []
    min_l = []
    for i in range(len(data)):
        for d in data[i]:
            max_l.append(np.max(d[0]))
            min_l.append(np.min(d[0]))

    data_max = max(max_l)
    data_min = min(min_l)

    for i in range(len(data)):
        for j in range(len(data[i])):
            data[i][j][0] = x_min + ((data[i][j][0]- data_min)/(data_max-data_min))*(x_max-x_min) #6.hafta slaytından
            
    return data

train_set , test_set = read_data_set()

input_size = 4 #giriş vektör sayısı
layer1 = 20 #gizli katman 1 nöron sayısı
layer2 = 20 #gizli katman 2 nöron sayısı
output_size = 3 #çıkış katmanı nöron saysısı
layers = [layer1, layer2]
a = 0.9 # transfer fonksiyonu katsayısı
m = 0.6 #momentum katsayısı
error_limit_train = 0.1 #mean squeared error
error_limit_test = 0.01 #max error

learning_rates = [0.2, 0.2, 0.2] # her katman için öğrenme katsayıları

test_1 = MultiLayerPerceptron(input_size, output_size, layers,learning_rates, a, m, error_limit_train, error_limit_test)
test_1.train(train_set)

test_1.test(test_set)

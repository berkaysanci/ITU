
import matplotlib.pyplot as plt
import numpy as np
from numpy.random.mtrand import shuffle
from mlp_lib import MultiLayerPerceptron
import datetime

np.random.seed(1)

img_1 = np.array([  [1,1,0,0,0,0,0,0,1,1],
                    [1,1,0,1,1,1,1,0,1,1],
                    [1,1,0,1,1,1,1,0,1,1],
                    [1,1,0,1,1,1,1,0,1,1],
                    [1,1,0,0,0,0,0,0,1,1],])

img_2 = np.array([  [1,1,0,0,0,0,0,0,1,1],
                    [1,1,1,1,0,0,1,1,1,1],
                    [1,1,1,1,0,0,1,1,1,1],
                    [1,1,1,1,0,0,1,1,1,1],
                    [1,1,1,1,0,0,1,1,1,1],])

img_3 = np.array([  [0,0,0,0,0,0,0,0,0,0],
                    [0,1,1,1,0,0,1,1,1,0],
                    [0,1,1,1,0,0,1,1,1,0],
                    [0,1,1,1,0,0,1,1,1,0],
                    [0,1,1,1,0,0,1,1,1,0],])

img_4 = np.array([  [1,1,1,0,0,1,1,1,0,0],
                    [1,1,1,0,0,1,1,1,0,0],
                    [1,1,1,0,0,0,0,0,0,0],
                    [1,1,1,0,0,1,1,1,0,0],
                    [1,1,1,0,0,1,1,1,0,0],])

def deffect(img, raito, deffect_type):
    if deffect_type == 0:
        noisy = img + raito * np.random.rand(5, 10)
        noisy = noisy/noisy.max()
        return noisy

    elif deffect_type == 1:
        x = int(raito/0.2)
        if x == 0:
            x = 1
        rand_image = np.array(img)
        for i in range(x):
            rand_index = np.random.randint(0, 50)
            rand_image[int(rand_index % 5), int(rand_index / 5)] = np.where(
                                            rand_image[int(rand_index % 5), int(rand_index / 5)] == 1,
                                            0.0, 1.0)
        return rand_image
        
def create_train_set(img_list, size, deffect_ratio):
    fig, axs = plt.subplots(len(img_list),size + 1)
    fig.suptitle("Train Set")
    data_set = []
    for i in range(len(img_list)):
        d_type = np.zeros((4,1))
        d_type[i] = 1
        data_set.append([img_list[i],d_type]) #orjinal resmi ekliyoruz
        axs[i][0].imshow(img_list[i], cmap='gray')
        for j in range(size):
            t = np.mod(j,2)
            def1 = deffect(img_list[i], deffect_ratio , t)
            data_set.append([def1, d_type])
            axs[i][j + 1].imshow(def1, cmap='gray')
    plt.show()
    return data_set

def create_test_set(img_list, size, deffect_ratio):
    fig, axs = plt.subplots(len(img_list),size)
    fig.suptitle("Test Set")
    data_set = []
    for i in range(len(img_list)):
        d_type = np.zeros((4,1))
        d_type[i] = 1
        for j in range(size):
            t = np.mod(j,2)
            def1 = deffect(img_list[i], deffect_ratio, t)
            data_set.append([def1, d_type])
            axs[i][j].imshow(def1, cmap='gray')
    plt.show()
    return data_set

def convert_to_input(data_set):
    for i in range(len(data_set)):
        data_set[i][0] = np.reshape(data_set[i][0],(50,1))

    return data_set

def scale_inputs(data,scale_range):
    x_min = min(scale_range)
    x_max = max(scale_range)

    max_l = []
    min_l = []
    for d in data:
        max_l.append(np.max(d[0]))
        min_l.append(np.min(d[0]))

    data_max = max(max_l)
    data_min = min(min_l)

    for i in range(len(data)):
        data[i][0] = x_min + ((data[i][0] - data_min)/(data_max-data_min))*(x_max-x_min) #6.hafta slaytından

    return data

img_list =[img_1,img_2,img_3,img_4]

deffect_size = 2 # img_list'deki her eleman için kaç tane hatalı resim üreteceği.  orjinal resim + deffect_size kadar veri döndürür
deffect_raito = 0.5
data_set = create_train_set(img_list, deffect_size, deffect_raito)

shuffle(data_set) 

input_size = 50 #giriş vektör sayısı
layer1 = 50 #gizli katman 1 nöron sayısı
layer2 = 50 #gizli katman 2 nöron sayısı
output_size = 4 #çıkış katmanı nöron saysısı
layers = [layer1, layer2]
a = 0.5 # transfer fonksiyonu katsayısı
m = 0.6 #momentum katsayısı
error_limit_train = 0.001 #mean squeared error
error_limit_test = 0.01 #max error

learning_rates = [0.5, 0.5, 0.5] # her katman için öğrenme katsayıları

data_set = convert_to_input(data_set)
data_set = scale_inputs(data_set,[0.1, 0.9])
test_1 = MultiLayerPerceptron(input_size, output_size, layers, learning_rates, a, m, error_limit_train, error_limit_test)
# test_1.use_adaptive_learning()
test_1.train(data_set)

deffect_size = 10 # img_list'deki her eleman için kaç tane hatalı resim üreteceği.  orjinal resim + deffect_size kadar veri döndürür
deffect_raito = 0.5

data_set_test = create_test_set(img_list, deffect_size, deffect_raito)

data_set_test = convert_to_input(data_set_test)
data_set_test = scale_inputs(data_set_test,[0.1, 0.9])
test_1.test(data_set_test)

"""adptif öğrenme
"""
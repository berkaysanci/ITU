import numpy as np
from numpy.random.mtrand import shuffle

np.random.seed(1)
import copy
import datetime
class MultiLayerPerceptron():
    
    def __init__(self, input_size, output_size, secret_layer_size, learning_rate, function_coff, momentum, error_limit_train, error_limit_test):
        self.input_size = input_size
        self.output_size = output_size
        self.secret_layer_size = secret_layer_size
        self.learning_rate = learning_rate
        self.func_coff = function_coff
        self.momentum = momentum
        self.error_limit_train = error_limit_train
        self.error_limit_test = error_limit_test
        self.w = None
        self.pre_w = None
        self.use_tanh_f = False
        # adaptive öğrenme hızı
        self.adap_lr = []
        self.use_adap = False
        self.pre_d = None
        self.a = 1.2
        self.b = 0.8

        self.create_w()

    def create_w(self):
        w = []
        w_in = np.random.uniform(low=-0.1, high=0.1, size=(self.secret_layer_size[0], self.input_size + 1))
        w.append(w_in)
        for i in range(len(self.secret_layer_size)-1):
            w_temp = np.random.uniform(low=-0.1, high=0.1, size=(self.secret_layer_size[i+1],self.secret_layer_size[i]+1))
            w.append(w_temp)
        w_out = np.random.uniform(low=-0.1, high=0.1, size=(self.output_size, self.secret_layer_size[-1] + 1))
        w.append(w_out)
        self.w = w
        self.pre_w = copy.deepcopy(self.w)

    def use_adaptive_learning(self):
        self.use_adap = True

    def use_tanh(self):
        self.use_tanh_f = True

    def use_logistic(self):
        self.use_tanh_f = False

    def __tanh_func(self, v):
        return np.tanh(v*self.func_coff)

    def __derv_tanh_func(self, v):
        return self.func_coff/np.cosh(v*self.func_coff)


    def __logistic_func(self, v):
        return 1.0/(1.0 + np.exp(-v*self.func_coff))

    def __derv_logistic_func(self, v):
        return self.__logistic_func(v)*(1 - self.__logistic_func(v)) #wikipedia

    def add_bias(self, vec):
        return np.vstack((vec, np.ones((1,1))))

    def normalize(self,data):
        pass
    
    def train(self, data, max_iter = 1000):
        if self.use_tanh_f:
            func = self.__tanh_func
            derv_func = self.__derv_tanh_func
        else:
            func = self.__logistic_func
            derv_func = self.__derv_logistic_func

        start = datetime.datetime.now()
        print("----------Training Started--------------")
        iter_cnt = 0
        while (True):
            iter_error = []
            for i in range(len(data)):
                x , yd = data[i]
                # x = normalize_inputs(input_d)

                v = []
                y = [x]
                input_x = x

                #ileri yön
                for i in range(len(self.w)):
                    input_x_ = np.vstack((input_x, np.ones((1,1))))  #bias ekliyoruz
                    result_v = np.dot(self.w[i],input_x_) # v vektrömüz
                    v.append(result_v)
                    result_y = func(result_v) #y vektörümüz
                    y.append(result_y)
                    input_x = result_y

                delta = []
                e = yd - y[-1] #yo = y[-1] 

                E = np.mean(0.5*(e*e)) # mean squared error
                iter_error.append(E)   #Hata hespaları saklıyoruz

                #ters yön yerel gradyan hesaplama
                for i in reversed(range(len(self.w))):
                    j = len(self.w) - 2 -i 

                    if i + 1  == len(self.w):
                        d = e * derv_func(v[i])
                        delta.append(d)
                    else:
                        w_T = np.transpose(self.w[i+1][:,:-1]) #bias sutununu atıp transpozunu alıyoruz
                        d = np.dot(w_T, delta[j]) * derv_func(v[i])
                        delta.append(d)

                #ağırlıkları güncelliyoruz
                w_temp = copy.deepcopy(self.w)
                for i in range(len(self.w)):
                    j = len(self.w) - i -1
                    y_T = np.transpose(np.vstack((y[i], np.ones((1,1))))) #bias ekleyip transpozunu alıyoruz

                    if self.use_adap:
                        if self.pre_d is not None:
                            for k in range(len(self.adap_lr[i])):
                                if self.pre_d[j][k]*delta[j][k] < 0:
                                    self.adap_lr[i][k] = self.adap_lr[i][k]*self.b
                                else:
                                    self.adap_lr[i][k] = self.adap_lr[i][k]*self.a
                            # print(self.adap_lr) 
                            w_temp[i] = self.w[i] + self.adap_lr[i] * np.dot(delta[j], y_T) + self.momentum*(self.w[i] - self.pre_w[i])

                        else:
                            self.adap_lr.append(np.full((w_temp[i].shape[0], 1), self.learning_rate[i]))
                            w_temp[i] = self.w[i] + self.learning_rate[i] * np.dot(delta[j], y_T) + self.momentum*(self.w[i] - self.pre_w[i])

                    else:
                        w_temp[i] = self.w[i] + self.learning_rate[i] * np.dot(delta[j], y_T) + self.momentum*(self.w[i] - self.pre_w[i])

             

                self.pre_w = copy.deepcopy(self.w)
                self.pre_d = copy.deepcopy(delta)
                self.w = w_temp
                            
            # iterasyon sonu tüm verileri ağa sürdük
            iter_cnt = iter_cnt + 1  #iterasyon sayacımız

            mean_error = np.mean(iter_error)
            max_error = np.max(iter_error)

            print("Iteration Count: ",iter_cnt, end = "\r")

            if max_error < self.error_limit_train:
                print("Itereation Count:", iter_cnt)
                print("Max Error:", max_error)
                print("Mean Error:", mean_error)
                break
        
            if iter_cnt > max_iter:
                print("iteration limit exceed")
                print("Max Error:", max_error)
                break
        end = datetime.datetime.now()
        d = end-start
        print("Training time:", d.seconds, "sec", d.microseconds/1000 , "ms")
        print("-----------Training Finished-----------")

    def test(self, data):

        if self.use_tanh_f:
            func = self.__tanh_func
            derv_func = self.__derv_tanh_func
        else:
            func = self.__logistic_func
            derv_func = self.__derv_logistic_func

        print("-------------Test Started--------------")
        true_cnt = 0
        yo = []
        for i in range(len(data)):
            x , yd = data[i]
            y = [x]
            input_x = x
            for i in range(len(self.w)):
                input_x_ = np.vstack((input_x, np.ones((1,1))))  #bias ekliyoruz
                result_v = np.dot(self.w[i] ,input_x_) # v vektrömüz
                result_y = func(result_v) #y vektörümüz
                y.append(result_y)
                input_x = result_y

            e = yd - y[-1] #yo = y[-1] 
            yo.append(y[-1][0])
            E = 0.5*(e*e)

            if np.max(E) < self.error_limit_test:
                true_cnt = true_cnt + 1

        print("True: ", true_cnt)
        print("False: ", len(data)-true_cnt)
        print("Error: %", 100*((len(data)-true_cnt)/len(data)))
        print("---------------Test Finished-----------")
        return yo


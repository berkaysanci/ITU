import numpy as np
import math

from numpy.lib.function_base import append
np.set_printoptions(precision=4)
np.random.seed(3) # Random değerleri sabitlemek için seed kullanıyoruz.

def func(x1,x2,x3): #Fonskisyonumuz
    return 0.5*x1*x2 + (x2**2)*math.exp(-x3)

def createSet(points): # Nokta kümesi yaratmak için
    out = np.empty((0,2))
    for i in range(len(points)):
        result = func(points[i][0],points[i][1],points[i][2]) #fonskiyonumuzdan değerlerini aliyoruz.
        points[i].append(1) #bias terimimiz vektöre ekiyoruz
        p = [points[i],result] #noktazın alt ya da üste olduğu belirleme için bir değişken ekliyoruz.
        out = np.vstack((out,p)) 
    return out

number_of_point = 10 #x^3 kadar nokta oluşturur

x = []

lin = np.linspace(0, 1.0, num=number_of_point ) # tüm nokta kombinasyonlarının olduğu küme oluşturyoruz
for i in lin:
    for j in lin:
        for k in lin:
            x.append([i,j,k])

point_cloud = createSet(x) #nokta setimizi oluşturuyoruz

print("point_cloud:\n",point_cloud)

train_points = point_cloud[:700] #Noktalarımızı eğitim ve test kümesi olarak iki gruba ayırıyoruz
test_points = point_cloud[700:]

print("train_points:\n", train_points)
print("test_points:\n", test_points)


print("\n---------Training Started -------\n")
w = np.random.rand(4)  # rastgele ilk ağırlıklar oluşturuyoruz
c = 0.7
Error = 0.05

print("w_0:",w) #ilk ağırlıklarımızı yazdırıyoruz.
iter_cnt = 0
preE=0
a = 0.7 
while(True):
    E = [] 
    for i in range(0,len(train_points)): #eğitim kümesindeki tüm noktalar için işlemi tekrarlıyoruz.
        v = np.dot(w,train_points[i][0]) # ağırlıklarımız ile x leri çarpıyoruz.
        y = math.tanh(v*a) #aktivasyon fonksiyonundan geçiriyoruz
        expected = train_points[i][1]
        # print("R:",expected,v)
        e = expected - y
        
        tarnsfer_func = (a*(1/(math.cosh(v)))**2) #optimazasyon için aktivasyon fonksiyonunun türevini kullanıyoruz
        
        w = w + c*e*np.array(train_points[i][0])*tarnsfer_func #ağırlıkarı güncelliyoruz.
        
        E.append(0.5*e**2) #Hata matrisimize hataları ekliyoruz
       
    iter_cnt = iter_cnt + 1  #iterasyon sayacımız

    print("w_{}:".format(iter_cnt) ,w) # ağırlıkları yazdırıyoruz.

    # print("E:",E)
    print("Sum E:",sum(E))
    
    Ea = abs(sum(E)-preE) #bağıl hata kullanıyoruz
    preE = sum(E)
    # print(Ea)
    if Ea < Error:
        break
    if iter_cnt > 100:
        print("iteration limit exceed")
        break

print("\n---------Test Started -------\n")

true_cnt = 0
for i in range(0,len(test_points)):
    v = np.dot(w,test_points[i][0]) # ağırlıklarımız ile x leri çarpıyoruz.
    y = math.tanh(v*0.7)
    expected = test_points[i][1]  #beklenen sonucumuz.
    e = expected - y
    Err = 0.5*e**2
    
    #sonucumuzu kontrol ediyoruz.
    if Err < Error:
        print("True", Err)
        true_cnt = true_cnt + 1
    else:
        print("False",Err)


print("\n---------Test Results -------\n")

print("True Count:", true_cnt,"\n")

print("False Count:", len(test_points)-true_cnt,"\n")

print("True Percent:", (true_cnt/len(test_points))*100,"% \n")
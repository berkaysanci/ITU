import numpy as np
import copy
import matplotlib.pyplot as plt


#noktalarımızı tanımlıyoruz.
points = np.array([[[0, -1],1],[[0, 0],1],[[0, 1],1],[[1, -1],1],[[1, 0],1],[[1, 1],1],[[-1, -1],1],[[-1, 0],1],[[-1, 1],1],
                    [[-3, 3],-1],[[-3, 1],-1],[[-3, 0],-1],[[-3, -1],-1],[[-3, -3],-1],[[-1, 3],-1],[[-1, -3],-1],
                    [[0, 3],-1],[[0, -3],-1],[[1, 3],-1],[[1, -3],-1],[[3, 3],-1],[[3, 1],-1],[[3, 0],-1],[[3, -1],-1],
                    [[3, -3],-1],[[-2, 3],-1],[[-3, 2],-1],[[-3, -2],-1],[[-2, -3],-1],[[2, 3],-1],[[3, 2],-1],[[3, -2],-1],[[2, -3],-1]])

# noktaları çizdirme
for i in range(len(points)):
    if points[i][1] == 1: # türü 1 olan noktalarımızı kırmızı olarak çizdiriyoruz.
        plt.plot(points[i][0][0],points[i][0][1],'ro')
    else: # türü -1 olan noktalarımızı mavi olarak çizdiriyoruz.
        plt.plot(points[i][0][0],points[i][0][1],'bo')
plt.show()

# Lineer ayrıştırna testini perceptron ile yapıyoruz.

test_points = copy.deepcopy(points) #noktalarımızı yeni kümeye kopyalıyoruz.
for i in range(len(test_points)):
    test_points[i][0].append(1) #bias ekliyoruz.

print("\n---------Test Started -------\n")

w = np.random.rand(3)  #başlangıç için rastgele ağırlıklar seçiyoruz.
c = 0.5 # öğrenme katsayımız

iter_cnt = 0
iter_limit = 100
while(True):
    true_cnt = 0 #doğru sayacımızı her döngüde sıfırlıyoruz

    for i in range(0,len(test_points)): #eğitim kümesindeki tüm noktalr için işlemi tekrarlıyoruz.

        result = np.dot(w,test_points[i][0]) # ağırlıkalrımız ile x leri çarpıyoruz.
        expected = test_points[i][1] #beklenen sonucumuz.

        #sonucumuzu 1 veya -1 olarak dönüştürüyoruz.
        if result > 0:
            result = 1
        else:
            result = -1

        if (result ==  expected): 
            true_cnt = true_cnt + 1 #eğer sonuç doğru ise devam ediyoruz.
        else: 
            w = w + c*0.5*(expected - result)*np.array(test_points[i][0]) # yanlış ise ağırlıkarı güncelliyoruz.

    iter_cnt = iter_cnt + 1 #iterasyon saycımız

    if true_cnt == len(test_points): #Tüm noktalar doğru ise eğitimi bitiriyoruz
        break

    if iter_cnt > iter_limit: #iterasyon limiti aşılırsa döngüyü bitiriyoruz.
        print("Iteration limit exceed!") #'iterasyon limiti aşıldı, linner ayrıştırılabilir değil' yazdırıyoruz.
        print("Not linear sepratable!")
        break

def phi(x): #ara katman fonkisyonumuzu tanımlıyoruz.
    return [x[0],x[1], x[0]**2 + x[1]**2]
    # return [x[0]**2,x[1]**2,np.sqrt(2)*x[0]*x[1]]

#yeni noktalar kümemizi çizdiriyoruz.
fig = plt.figure()
ax = fig.add_subplot(projection='3d')

for i in range(len(points)):
    t = points[i][1]
    x,y,z = phi(points[i][0])
    if t == 1:
        ax.scatter(x,y, z ,marker ='o', color = "red")
    else:
        ax.scatter(x,y, z, marker = '^', color = "blue")
plt.show()


#Rosenblatt’ın genlikte ayrık algılayıcısını phi fonksiyonumuz ile deniyoruz.

new_points = np.empty((0,2)) #phi fonksiyonundan çıkan noktalar için liste oluşturuyoruz.
for i in range(len(points)):
    x = phi(points[i][0])
    x.append(1)
    p = [x,points[i][1]] #nokta türünü listeye ekliyoruz.
    new_points = np.vstack((new_points,p)) #noktalarımızı listeye ekliyoruz.

print(new_points)

print("\n---------Test Started -------\n")
w = np.random.rand(4)  #başlangıç için rastgele ağırlıklar seçiyoruz.
c = 0.1 # öğrenme katsayımız

print("w_0:",w,"\n") #ilk ağırlıklarımızı yazdırıyoruz.

iter_cnt = 0
iter_limit = 100
while(True):

    true_cnt = 0 #doğru sayacımızı her döngüde sıfırlıyoruz.
    for i in range(0,len(new_points)): #eğitim kümesindeki tüm noktalar için işlemi tekrarlıyoruz.

        result = np.dot(w,new_points[i][0]) # ağırlıklarımızın phi fonksiyonu çıktıları
        expected = new_points[i][1] #beklenen sonucumuz.

        #sonucumuzu 1 veya -1 olarak dönüşütürüyoruz.
        if result > 0:
            result = 1
        else:
            result = -1

        if (result ==  expected): 
            true_cnt = true_cnt + 1 #eğer sonuç doğru ise devam ediyoruz.
        else: 
            w = w + c*0.5*(expected - result)*np.array(new_points[i][0]) # yanlış ise ağırlıkarı güncelliyoruz.

    iter_cnt = iter_cnt + 1 #iterasyon sayacımız

    print("w_{}:".format(iter_cnt) ,w) # ağırlıkları yazdırıyoruz.

    if true_cnt == len(new_points): #Tüm noktalar doğru ise eğitimi bitiriyoruz.
        break

    if iter_cnt > iter_limit: #iterasyon limiti aşılırsa döngüyü bitiriyoruz
        print("Iteration limit exceed!") #'iterasyon limiti aşıldı linner, ayrıştırılabilir değil' yazdırıyoruz.
        print("Not linear sepratable!")
        break

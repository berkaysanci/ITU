import numpy as np

def printArray(array):
    for i in range(0, len(array)):
        print("{"+str(array[i][0])+ " " + str(array[i][1])+"}")

np.set_printoptions(formatter=None)

np.random.seed(1010) # random değerleri sabitlemek için seed kullanıyoruz.


with open('points_1b.npy', 'rb') as f: #kayıtlı dosyadan noktalarımzı açıyoruz
    points_1 = np.load(f,allow_pickle=True)

with open('points_2b.npy', 'rb') as f:  #kayıtlı dosyadan noktalarımzı açıyoruz.
    points_2 = np.load(f,allow_pickle=True)

#noktalarımızı yazdırıyoruz.
print("points_1:\n", points_1,"\n")
print("points_2:\n", points_2,"\n")

point_cloud = np.concatenate((points_1,points_2)) # noktalarımızı tek bir listede birleştiriyoruz.

print("point_cloud:\n",point_cloud,"\n")

np.random.shuffle(point_cloud) #noktalarımızı karıştırıyoruz.

print("point_cloud_shuffled:\n", point_cloud,"\n")

train_points = point_cloud[:30] #karışmış listeden ilk 30 noktayı eğitim kümesi olarak
test_points = point_cloud[30:] #son 20 noktayı ise eğitim kümesi olarak alıyoruz.

print("train_points:\n", train_points,"\n")
print("test_points:\n", test_points,"\n")


print("\n---------Training Started -------\n")
w = np.random.rand(7)  #başlangıç için rastgele ağırlıklar seçiyoruz.
c = 0.5 # öğrenme katsayımız


print("w_0:",w,"\n") #ilk ağırlıklarımızı yazdırıyoruz.


iter_cnt = 0
iter_limit = 100
while(True):
    true_cnt = 0 #doğru sayacımızı her döngüde sıfırlıyoruz.

    for i in range(0,len(train_points)): #eğitim kümesindeki tüm noktalar için işlemi tekrarlıyoruz.

        result = np.dot(w,train_points[i][0]) # ağırlıklarımız ile x leri çarpıyoruz.
        expected = train_points[i][1] #beklenen sonucumuz.

        #sonucumuzu 1 veya -1 olarak dönüşütürüyoruz.
        if result > 0:
            result = 1
        else:
            result = -1


        if (result ==  expected): 
            true_cnt = true_cnt + 1 #eğer sonuç doğru ise devam ediyoruz.
        else: 
            w = w + c*0.5*(expected - result)*np.array(train_points[i][0]) # yanlış ise ağırlıkarı güncelliyoruz.

    iter_cnt = iter_cnt + 1 #iterasyon sayacımız

    print("w_{}:".format(iter_cnt) ,w) # ağırlıkları yazdırıyoruz.

    if true_cnt == len(train_points): #tüm noktalar doğru ise eğitimi bitiriyoruz.
        break

    if iter_cnt > iter_limit: #iterasyon limiti aşılırsa döngüyü bitiriyoruz.
        print("Iteration limit exceed!")
        break

print("\n---------Test Started -------\n")

true_cnt = 0
for i in range(0,len(test_points)):
    restult = np.dot(w,test_points[i][0]) # ağırlıklarımız ile x leri çarpıyoruz.
    expected = test_points[i][1] #beklenen sonucumuz.


    #sonucumuzu 1 veya -1 olarak dönüşütürüyoruz.
    if restult > 0:
        restult = 1
    else:
        restult = -1

    #sonucumuzu kontrol ediyoruz.
    if (restult ==  expected):
        true_cnt = true_cnt + 1
        print("True")
    else:
        print("False")


print("\n---------Test Results -------\n")

print("True Count:", true_cnt,"\n")

print("False Count:", len(test_points)-true_cnt,"\n")

print("True Percent:", (true_cnt/len(test_points))*100,"% \n")




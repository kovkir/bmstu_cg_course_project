import matplotlib.pyplot as plt

def build_graph(sizes_arr, time_without_omp, time_with_omp):

    fig1 = plt.figure(figsize = (10, 7))
    plot = fig1.add_subplot()
    plot.plot(sizes_arr, time_without_omp, label = "Без распараллеливания")
    plot.plot(sizes_arr, time_with_omp,    label = "C распараллеливанием")

    plt.legend()
    plt.grid()
    plt.title("Сравнение затраченного времени")
    plt.ylabel("Затраченное время (мс)")
    plt.xlabel("Размер сцены (кол-во клеток)")
    
    plt.show()
    

def compare_time():
    time_without_omp = [25, 152, 203, 227, 251, 296]
    time_with_omp    = [33, 92,  123, 163, 195, 242]

    sizes_arr = ["1x1", "10x10", "20x20", "30x30", "40x40", "50x50"]

    build_graph(sizes_arr, time_without_omp, time_with_omp)

if __name__ == "__main__":
    compare_time()

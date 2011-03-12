import csv

from numpy import *

from metrics import sim_distance

def kmeans(data, distance=sim_distance, k=3, iternum=100, err=0.0005):
    vecnum, veclen = data.shape
    minima = data.min(axis=0)
    maxima = data.max(axis=0)

    centroids = random.rand(k, veclen) * (maxima - minima) + minima
    clusters = None

    for t in range(iternum):
        distances = zeros(shape=(k, vecnum))
        for i in range(k):
            distances[i] = sum((data - centroids[i,:])**2, axis=1)

        clusters = distances.argmin(axis=0)
        clusters.shape = (vecnum, 1)

        old_centroids = centroids.copy()
        for i in range(k):
            cluster = where(clusters == i, 1, 0)
            if sum(cluster) > 0:
                centroids[i,:] = sum(data * cluster, axis=0) / sum(cluster)

        if abs(sum(centroids - old_centroids)) < err:
            break

    return centroids, clusters

if __name__ == "__main__":
    reader = csv.reader(open('iris-flower-dataset.csv', 'r'),
                        quoting=csv.QUOTE_NONNUMERIC)
    reader.next()

    data = array([row[:4] for row in reader])
    centroids, clusters = kmeans(data, k=3)

    writer = csv.writer(open('iris-flower-kmeans.csv', 'w'),
                        quoting=csv.QUOTE_NONNUMERIC)
    writer.writerow(["sepal-length", "sepal-width", "petal-length",
                     "petal-width", "species"])

    english = { 0 : "first", 1 : "second", 2 : "third" }
    for row, cluster in zip(data, clusters):
        writer.writerow(row.tolist() + [english[cluster[0]]])

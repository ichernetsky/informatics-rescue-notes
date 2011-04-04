import csv
import metrics

from numpy import *

def knn(item, dataset):
    klass = None
    nearest = None

    for row in dataset:
        next = metrics.sim_distance(item, row["features"])

        if nearest is None:
            nearest =  next
        elif next > nearest:
            klass = row["class"]
            nearest = next

    return klass

class Node(object):
    def __init__(self, location, left, right):
        self.location = location
        self.left = left
        self.right = right

def kdtree(points, depth=0):
    if not points.size:
        return None

    n, k = points.shape
    axis = depth % k

    points.sort(axis=axis)
    median = n // 2

    location = points[median]
    left = kdtree(points[:median], depth + 1)
    right = kdtree(points[median + 1:], depth + 1)
    node = Node(location, left, right)

    return node

if __name__ == "__main__":
    reader = csv.reader(open('iris-flower-dataset.csv', 'r'),
                        quoting=csv.QUOTE_NONNUMERIC)
    reader.next()

    dataset = list()
    for row in reader:
        dataset.append({ "features" : row[:4],
                         "class"    : row[4]   })

    print knn([4.5, 3.8, 1.2, 0.4], dataset)
    print kdtree(array([(2,3),
                        (5,4),
                        (9,6),
                        (4,7),
                        (8,1),
                        (7,2)]))

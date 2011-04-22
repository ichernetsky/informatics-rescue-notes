import csv
import metrics

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
    def __init__(self, location, kind, left, right):
        self.location = location
        self.kind = kind
        self.left = left
        self.right = right

def kdtree(dataset, depth=0):
    if not dataset:
        return None

    n = len(dataset)
    k = len(dataset[0]["features"])
    axis = depth % k
    median = n // 2

    dataset.sort(key=lambda row: row["features"][axis])

    location = dataset[median]["features"]
    kind = dataset[median]["class"]
    left = kdtree(dataset[:median], depth + 1)
    right = kdtree(dataset[median + 1:], depth + 1)

    return Node(location, kind, left, right)

if __name__ == "__main__":
    reader = csv.reader(open('iris-flower-dataset.csv', 'r'),
                        quoting=csv.QUOTE_NONNUMERIC)
    reader.next()

    dataset = list()
    for row in reader:
        dataset.append({ "features" : row[:4],
                         "class"    : row[4]   })

    print knn([4.5, 3.8, 1.2, 0.4], dataset)
    kdtree = kdtree(dataset)

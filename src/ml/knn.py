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

if __name__ == "__main__":
    reader = csv.reader(open('iris-flower-dataset.csv', 'r'),
                        quoting=csv.QUOTE_NONNUMERIC)
    reader.next()

    dataset = list()
    for row in reader:
        dataset.append({ "features" : row[:4],
                         "class"    : row[4]   })

    print knn([4.5, 3.8, 1.2, 0.4], dataset)

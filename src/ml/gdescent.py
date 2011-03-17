import operator

from numpy import *

def sinsin_gradient(vec):
    x, y = vec
    return array([cos(x) * sin(y),
                  sin(x) * cos(y)])

samples = [
    # (square, price)

    (1641.0, 112.0),
    (2232.0, 214.9),
    (1050.0, 109.9),
    (2496.0, 155.0),
    (1919.0, 117.0),
    (672.0,  74.9),
    (1448.0, 119.5),
    (1448.0, 122.0),
    (1662.0, 135.0),
    (1800.0, 159.9),
    (3900.0, 199.9),
    (1200.0, 47.9),
    (2016.0, 127.9),
    (2496.0, 167.5),
    (1735.0, 114.9),
    (2848.0, 153.0),
    (1257.0, 82.9),
    (1480.0, 154.9),
    (2496.0, 155.0),
    (1332.0, 69.9),
    (1792.0, 100.0),
    (2037.0, 179.9),
    (2488.0, 115.0),
    (2356.0, 130.0),
    (1919.0, 117.0),
    (2186.0, 178.9),
    (2070.0, 166.9),
    (1632.0, 169.9),
    (1735.0, 139.9),
    (1510.0, 169.9),
    (2344.0, 379.5)
]

def lg_gradient(p):
    def fn(x):
        return p[0] + p[1] * x

    result = array([0 for _ in range(len(p))])
    for x, y in samples:
        result += (fn(x) - y) * array([1.0, x])

    return result

def gdescent(initial, gradient_fn, step_fn=operator.sub,
             maxiter=1000, eps=0.01, precision=0.00001):

    x = initial
    for i in range(maxiter):
        old_x = x.copy()
        x = step_fn(old_x, eps * gradient_fn(x))

        if sum((x - old_x) ** 2) < precision:
            break

    return x

if __name__ == "__main__":
    print gdescent(array([2.0, -1.0]), sinsin_gradient,
                   step_fn=operator.add)
    print gdescent(array([20.0, 0.1]), lg_gradient, eps=0.00000001)

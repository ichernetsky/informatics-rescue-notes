from numpy import *

def z_jacobian(vec):
    x, y = vec
    return array([cos(x) * sin(y),
                  sin(x) * cos(y)])

def gdescent(x, jacobian, maxiter=1000, eps=0.01, precision=0.00001):
    for i in range(maxiter):
        old_x = x.copy()
        x = old_x + eps * jacobian(x)
        if sum((x - old_x) ** 2) < precision:
            break
    return x

if __name__ == "__main__":
    gdescent(array([2, -1]), z_jacobian)

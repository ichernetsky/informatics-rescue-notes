from math import sqrt

def sim_distance(vec1, vec2):
    assert len(vec1) == len(vec2)
    sum_of_squares = sum(pow(v1 - v2, 2)
                         for v1, v2 in zip(vec1, vec2))

    return 1 / (1 + sum_of_squares)

def sim_pearson(vec1, vec2):
    assert len(vec1) == len(vec2)

    n = len(vec1)
    if n == 0:
        return 0

    sum1, sum2 = sum(vec1), sum(vec2)
    sum1_of_squares = sum(pow(v, 2) for v in vec1)
    sum2_of_squares = sum(pow(v, 2) for v in vec2)
    sum_of_products = sum(v1 * v2 for v1, v2 in zip(vec1, vec2))

    num = sum_of_products - (sum1 * sum2 / n)
    den = sqrt((sum1_of_squares - pow(sum1, 2) / n) *
               (sum2_of_squares - pow(sum2, 2) / n))

    if den == 0:
        return 0

    r = num / den
    return r

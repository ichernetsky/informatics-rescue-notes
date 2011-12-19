class BloomFilter(object):
    def __init__(self, size, hashers):
        assert size > 0,     "size must be greater than zero"
        assert len(hashers), "at least one hash function must be provided"

        self.size    = size
        self.hashers = tuple(hashers)
        self.bitset  = 0L

    def add(self, item):
        for hasher in self.hashers:
            position = hasher(item) % self.size
            self.bitset |= 1 << position

    def __contains__(self, item):
        for hasher in self.hashers:
            position = hasher(item) % self.size
            if not (self.bitset & (1 << position)):
                return False
        return True

    def __repr__(self):
        return self.__str__()

    def __str__(self):
        binary = bin(self.bitset)
        binary = binary[2:]
        binary = '0' * (self.size - len(binary)) + binary
        return binary

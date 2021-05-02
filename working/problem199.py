import math
import itertools

N = 10

class Circle:
    next_index = 0 # class variable, same pointer to this variable across all objects of class
    def __init__(self, radius, is_exterior_circle):
        self.index = Circle.next_index
        Circle.next_index += 1
        self.radius = radius
        self.is_exterior_circle = is_exterior_circle
    def __str__(self):
        return str(self.index)
    def __eq__(self, rhs):
        return self.index == rhs.index
    def __gt__(self, rhs):
        return self.index > rhs.index
    def get_area_magnitude(self):
        #return math.pi * ((self.radius) ** 2)
        return self.radius ** 2

class CircleTriplet:
    def __init__(self, circle1, circle2, circle3):
        self.circles = sorted([circle1, circle2, circle3])
        self._hash = self.get_hash()
    def get_hash(self):
        return ",".join([str(c) for c in self.circles])
    def get_next_circle(self):
        k1 = 1/self.circles[0].radius
        if self.circles[0].is_exterior_circle:
            k1 = -k1
        k2 = 1/self.circles[1].radius
        if self.circles[1].is_exterior_circle:
            k2 = -k2
        k3 = 1/self.circles[2].radius
        if self.circles[2].is_exterior_circle:
            k3 = -k3
        new_k = k1 + k2 + k3 + 2 * math.sqrt(k1 * k2 + k2 * k3 + k3 * k1)
        new_radius = 1/new_k
        new_circle = Circle(new_radius, is_exterior_circle = False)
        return new_circle
    def get_next_circle_triplets(self):
        new_circle = self.get_next_circle()
        next_circle_triplets = []
        for circle_pairs in itertools.combinations(self.circles, 2):
            new_triplet = CircleTriplet(circle_pairs[0], circle_pairs[1], new_circle)
            next_circle_triplets.append(new_triplet)
        return next_circle_triplets

if __name__ == "__main__":
    circles = [Circle(1, is_exterior_circle = True),
               Circle(2*math.sqrt(3) - 3, is_exterior_circle = False)]
    remaining_area = circles[0].get_area_magnitude()
    remaining_area -= 3 * circles[1].get_area_magnitude()

    triplets = {}

    triplet0 = CircleTriplet(circles[0], circles[1], circles[1])
    triplet1 = CircleTriplet(circles[1], circles[1], circles[1])
    triplets[triplet0.get_hash()] = triplet0
    triplets[triplet1.get_hash()] = triplet1

    curr = {triplet0.get_hash(): 3, triplet1.get_hash(): 1}
    prev = {}

    for curr_hash, curr_freq in curr.items():
        triplet = triplets[curr_hash]
        remaining_area -= curr_freq * triplet.get_next_circle().get_area_magnitude()

    for iteration in range(N-1):
        curr, prev = {}, curr
        for prev_hash, prev_freq in prev.items():
            prev_triplet = triplets[prev_hash]
            for new_circle_triplet in prev_triplet.get_next_circle_triplets():
                new_circle_triplet_hash = new_circle_triplet.get_hash()
                if not new_circle_triplet_hash in triplets:
                    triplets[new_circle_triplet_hash] = new_circle_triplet
                    curr[new_circle_triplet_hash] = 0
                curr[new_circle_triplet_hash] += prev_freq
        for curr_hash, curr_freq in curr.items():
            triplet = triplets[curr_hash]
            remaining_area -= curr_freq * triplet.get_next_circle().get_area_magnitude()
    print(remaining_area / circles[0].get_area_magnitude())

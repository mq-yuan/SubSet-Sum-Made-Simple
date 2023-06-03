import random
import os


def generate_random_numbers(length, start, end):
    numbers = set()
    while len(numbers) < length:
        number = random.randint(start, end)
        numbers.add(number)
    return list(numbers)


test_samples_dir = "./samples3"
for num in range(1, 1001):
    if not os.path.exists(test_samples_dir):
        os.makedirs(test_samples_dir)
    with open(os.path.join(test_samples_dir, f"p{num}.txt"), "a+") as f:
        n = random.randint(100, 10000)
        f.writelines(f"{random.randrange(100, 10000)}, {n}\n")
        random_numbers = generate_random_numbers(n, 0, 1000000)
        for i in range(n):
            f.writelines(f"{random_numbers[i]}\n")

import subprocess
import time
import os

input_dir_path = "./samples3"
input_dir = os.listdir(input_dir_path)
programe_list = [
    "./dynamic.exe",
    "./test.exe",
]
execution_time = []

for name in programe_list:
    start_time = time.time()
    for idx in range(20):
        input_name = input_dir[idx]
        subprocess.call([name, os.path.join(input_dir_path, input_name)])
    execution_time.append(time.time() - start_time)

print(f"total samples is {len(input_dir)}")
for idx, name in enumerate(programe_list):
    print(f"{name} spend {execution_time[idx]:.4f} seconds")

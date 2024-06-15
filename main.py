import matplotlib.pyplot as plt
import re

methods = []
times = []
memories = []

with open('/Users/dawid/ClionProjects/maximum_cut_in_graph/results.txt', 'r') as data:
    lines = data.readlines()
data.close()

count = 0

for line in lines:
    if "Metoda" in line:
        method = re.findall(r"Metoda: (\w+)", line)[0]
        methods.append(method)
        print(method)
    values = re.findall(r"\d+", line)

    if values:
        if count % 2 == 0:
            times.extend(values)
        else:
            memories.extend(values)
        count += 1
        print(values)

times = [int(time) for time in times]
memories = [int(memory) for memory in memories]

fig, axs = plt.subplots(2, 1, figsize=(10, 15))

axs[0].bar(methods, times, color='blue')
axs[0].set_title('Czas wykonania')
axs[0].set_ylabel('Czas (ms)')
axs[0].set_xlabel('Metoda')

axs[1].bar(methods, memories, color='green')
axs[1].set_title('Zużycie pamięci')
axs[1].set_ylabel('Pamięć (KB)')
axs[1].set_xlabel('Metoda')

plt.tight_layout()
plt.show()

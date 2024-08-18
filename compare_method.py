import matplotlib.pyplot as plt

methods = []
iterations = []
convergence = []
cut_values = []
times = []
resources = []

# dodać kolory dla więcej metod
bar_colors = ['tab:red', 'tab:blue', 'tab:orange', 'tab:purple', 'tab:green']

file = open('/Users/dawid/ClionProjects/maxcut/results.txt', 'r')

for line in file:
    parts = line.strip().split(':')

    if (len(parts) < 2):
        continue

    name, value = parts[0].strip(), parts[1].strip()

    if (name == "Metoda"):
        methods.append(value)
    elif (name == "Iteracje"):
        iterations.append(int(value))
    elif (name == "Zbieżność"):
        convergence.append(int(value))
    elif (name == "Zasoby"):
        resources.append(int(value))
    elif (name == "Cut"):
        cut_values.append(int(value))
    elif (name == "Czas"):
        times.append(int(value))

fig1, ax1 = plt.subplots()
ax1.bar(methods, cut_values, label=methods, color=bar_colors)
ax1.set_ylabel('Wartość')
ax1.set_title('Porównanie metod względem cięcia')
ax1.set_xticks([])
ax1.legend(title='Metody')

fig2, ax2 = plt.subplots()
ax2.bar(methods, convergence, label=methods, color=bar_colors)
ax2.set_ylabel('Powtórzenia iteracji')
ax2.set_title('Porównanie metod względem zbieżności')
ax2.set_xticks([])
ax2.legend(title='Metody')

fig3, ax3 = plt.subplots()
ax3.bar(methods, times, label=methods, color=bar_colors)
ax3.set_ylabel('Czas(s)')
ax3.set_title('Porównanie metod względem czasu wykonania')
ax3.set_xticks([])
ax3.legend(title='Metody')

fig4, ax4 = plt.subplots()

for i in range(len(methods)):
    oiterations = list(range(1, iterations[i] + 1))
    objective_values = [resources[i] / (j + 1) * (i + 1) for j in range(iterations[i])]
    ax4.plot(oiterations, objective_values, '-', color=bar_colors[i], label=methods[i], linewidth=1.5)

ax4.set_ylim(0, max(resources) * 1.1)
ax4.legend(title='Metody')
ax4.grid(True)

plt.tight_layout()

plt.show()

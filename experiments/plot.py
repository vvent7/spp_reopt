import sys
import pandas as pd

import matplotlib.pyplot as plt


def plot_graph(instance):
    data = pd.read_csv('out/output.csv')
    rows = data[data['instance'] == instance]
    print(rows)
    rows.plot(x = 'group', style=['p-','d-', 'o-', '.--', 's:','*:','h-', '+-'])
    plt.xticks(rows['group'])
    plt.xlabel('Grupo')
    plt.ylabel('Tempo')
    plt.show()
    plt.gcf.canvas.manager.set_window_title(instance)

    



if len(sys.argv) < 2:
    print('Missing instance name!')
else:
    plot_graph(sys.argv[1])

# plot_graph('planar')

plt.close("all")
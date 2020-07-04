import networkx as nx
import matplotlib.pyplot as plt
color_map=[]
G = nx.DiGraph()
edges = nx.read_edgelist('epinions.txt',create_using=G)
G.add_edges_from(edges.edges())
plt.figure(figsize=(9, 9))
Gc = max(nx.strongly_connected_components(G), key=len)
for node in G:
    if node in Gc:
        color_map.append('red')
    else:
        color_map.append('blue')
nx.draw_networkx(G,with_labels=False,node_color=color_map)
plt.show()


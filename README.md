Compile

g++ PUL.cpp -O3 -std=c++11 -o pul

Run

./pul

Graph File  Format

The second line  must contains V,E the number of vertices and edges of the graph.

Then V lines follow.
Each line describes edges from a certain vertex, u, to its successors, v_i, in the following format.
u v_i

Query File Format
Each line contains a query, Reach(u, v), in the following format.
u v 

Please note the graph must be a DAG.

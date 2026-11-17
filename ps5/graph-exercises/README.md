# BFS 和 DFS 练习

- `graph.hpp`：已实现的有向图邻接表；无向边可以分别添加两个方向。
- `bfs.hpp`：自行实现广度优先搜索，返回父节点与最短距离。
- `dfs.hpp`：自行实现深度优先搜索，返回父节点与发现顺序。
- `test_graph_search.cpp`：检查连通性、父节点、BFS 距离和 DFS 发现顺序。

节点编号是 `0..V-1`。两种搜索均约定 `parent[source] = source`，不可达节点的 parent 为 `-1`；BFS 对不可达节点的 distance 也为 `-1`。邻居按添加顺序保存；DFS 的同层访问顺序不唯一，测试不要求固定顺序。source 不在图中时，搜索函数应抛出 `std::out_of_range`。

在此目录编译并运行：

```sh
c++ -std=c++17 -Wall -Wextra -pedantic test_graph_search.cpp -o graph_search_test
./graph_search_test
```

未完成的函数会抛出 `std::logic_error`，测试程序会报告对应 TODO 并以非零状态退出。完成 BFS 或 DFS 后，可以分别看到该部分的检查结果。

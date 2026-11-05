# MIT 6.006：2020 春季作业

资料来自 MIT OpenCourseWare 的 [Assignments](https://ocw.mit.edu/courses/6-006-introduction-to-algorithms-spring-2020/pages/assignments/) 页面。

每个 `ps0`–`ps8` 文件夹包含：

- `originals/`：题目 PDF 和官方原始模板 ZIP。
- `work/`：解压后供你编辑的模板文件。Python 题目的 `tests.py` 是官方提供的样例测试。
- `solutions/`：官方参考答案 PDF；建议独立完成后再查看。
- `problem-session/`：对应的 Problem Session 练习题和官方解答。Session 1–9 分别放在 `ps0`–`ps8`；这些练习与作业是不同的材料。

Problem Session 资料来自 MIT OpenCourseWare 的 [Practice Problems](https://ocw.mit.edu/courses/6-006-introduction-to-algorithms-spring-2020/pages/practice-problems/) 页面。例如，Stone Searching 在 `ps1/problem-session/session2-questions.pdf` 中。

官方原始 ZIP 保留完整内容；`work/` 排除了 macOS 元数据和临时文件。PS8 没有编程部分，模板只包含 LaTeX 文件。

| 作业 | 编程文件 | 说明 |
| --- | --- | --- |
| PS0 | `count_long_subarray.py` | 最长递增连续子数组的数量；题目 0-6 |
| PS1 | `Doubly_Linked_List_Seq.py` | 实现题目指定的双向链表操作 |
| PS2 | `satisfying_booking.py` | 实现符合条件的预订安排 |
| PS3 | `count_anagram_substrings.py` | 计数符合条件的异位词子串 |
| PS4 | `tastiest_slice.py` | 实现所需的数据结构操作与算法；其余 AVL 文件为辅助代码 |
| PS5 | `solve_tilt.py` | 图搜索：解决倾斜拼图 |
| PS6 | `johnson.py` | 实现 Johnson 最短路径算法 |
| PS7 | `short_company.py` | 动态规划 |
| PS8 | 无 | 没有需要提交的编程部分 |

具体要求、证明、复杂度和函数约定以对应题目 PDF 为准。多个模板用 `YOUR CODE HERE` 标出填写位置；PS1 则需要按题目要求实现指定方法。

从 PS0 开始：

```sh
cd ~/MIT-6.006-Spring-2020/ps0/work
vim count_long_subarray.py
python3 -m unittest -v tests
```

未经实现的模板出现测试失败是正常的。提供的测试仅是样例，通过它们不能保证满足所有边界条件或复杂度要求。理论题的 LaTeX 模板位于 `work/`；也可使用自己方便的方式书写证明。

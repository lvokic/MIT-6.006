def count_long_subarray(A: tuple[int, ...]) -> int:
    """
    Input:  A     | Python Tuple of positive integers
    Output: count | number of longest increasing subarrays of A
    """
    count = 0
    ##################
    cur_len = 1
    max_len = 1
    for i in range(1, len(A)):
        if A[i] > A[i - 1]:
            cur_len += 1
        else:
            if (cur_len == max_len):
                count += 1
            elif (max_len < cur_len):
                max_len = cur_len
                count = 1
            cur_len = 1
    if cur_len == max_len:
        count += 1
    elif cur_len > max_len:
        count = 1
    ##################
    return count

import queue

N = 10**9
#N = 6

if __name__ == "__main__":
    overall_cost = 5

    alphabet_size = 2
    free_node_freq = {1: 1, 4: 1}
    pq = queue.PriorityQueue()
    pq.put(1)
    pq.put(4)

    while alphabet_size < N:
        n_remaining_chars = N - alphabet_size
        free_node_cost = pq.get()
        n_free_nodes = free_node_freq.pop(free_node_cost, None)
        n_new_nodes = min(n_remaining_chars, n_free_nodes)

        print(free_node_cost, n_new_nodes, n_remaining_chars)

        overall_cost += (free_node_cost + 5) * n_new_nodes

        for new_cost in [free_node_cost + 1, free_node_cost + 4]:
            if not new_cost in free_node_freq:
                free_node_freq[new_cost] = 0
                pq.put(new_cost)
            free_node_freq[new_cost] += n_new_nodes

        alphabet_size += n_new_nodes


    print(overall_cost)

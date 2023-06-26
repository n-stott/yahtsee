def read_file(filename):
    res = []
    with open(filename, 'r') as file:
        for line in file:
            data = line.strip().split('|')[-1]
            data = data.split()
            data = [float(d) for d in data]
            res.append(data)
    return res


cpp = read_file('cpp')
lsp = read_file('lsp')

assert(len(cpp) == len(lsp))
for i in range(len(cpp)):
    assert(len(cpp[i]) == len(lsp[i]))
    for j in range(len(cpp[i])):
        assert(abs(cpp[i][j] - lsp[i][j]) < 0.0001)
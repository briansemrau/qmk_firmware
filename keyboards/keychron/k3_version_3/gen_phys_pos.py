import json

data_str = """
[
    {"matrix":[0, 0], "x":0, "y":0},
    {"matrix":[0, 1], "x":1, "y":0},
    {"matrix":[0, 2], "x":2, "y":0},
    {"matrix":[0, 3], "x":3, "y":0},
    {"matrix":[0, 4], "x":4, "y":0},
    {"matrix":[0, 5], "x":5, "y":0},
    {"matrix":[0, 6], "x":6, "y":0},
    {"matrix":[0, 7], "x":7, "y":0},
    {"matrix":[0, 8], "x":8, "y":0},
    {"matrix":[0, 9], "x":9, "y":0},
    {"matrix":[0,10], "x":10, "y":0},
    {"matrix":[0,11], "x":11, "y":0},
    {"matrix":[0,12], "x":12, "y":0},
    {"matrix":[0,13], "x":13, "y":0},
    {"matrix":[0,14], "x":14, "y":0},
    {"matrix":[0,15], "x":15, "y":0},

    {"matrix":[1, 0], "x":0, "y":1},
    {"matrix":[1, 1], "x":1, "y":1},
    {"matrix":[1, 2], "x":2, "y":1},
    {"matrix":[1, 3], "x":3, "y":1},
    {"matrix":[1, 4], "x":4, "y":1},
    {"matrix":[1, 5], "x":5, "y":1},
    {"matrix":[1, 6], "x":6, "y":1},
    {"matrix":[1, 7], "x":7, "y":1},
    {"matrix":[1, 8], "x":8, "y":1},
    {"matrix":[1, 9], "x":9, "y":1},
    {"matrix":[1,10], "x":10, "y":1},
    {"matrix":[1,11], "x":11, "y":1},
    {"matrix":[1,12], "x":12, "y":1},
    {"matrix":[1,13], "x":13, "y":1, "w":2},
    {"matrix":[1,15], "x":15, "y":1},

    {"matrix":[2, 0], "x":0, "y":2, "w":1.5},
    {"matrix":[2, 1], "x":1.5, "y":2},
    {"matrix":[2, 2], "x":2.5, "y":2},
    {"matrix":[2, 3], "x":3.5, "y":2},
    {"matrix":[2, 4], "x":4.5, "y":2},
    {"matrix":[2, 5], "x":5.5, "y":2},
    {"matrix":[2, 6], "x":6.5, "y":2},
    {"matrix":[2, 7], "x":7.5, "y":2},
    {"matrix":[2, 8], "x":8.5, "y":2},
    {"matrix":[2, 9], "x":9.5, "y":2},
    {"matrix":[2,10], "x":10.5, "y":2},
    {"matrix":[2,11], "x":11.5, "y":2},
    {"matrix":[2,12], "x":12.5, "y":2},
    {"matrix":[2,13], "x":13.5, "y":2, "w":1.5},
    {"matrix":[2,15], "x":15, "y":2},

    {"matrix":[3, 0], "x":0, "y":3, "w":1.75},
    {"matrix":[3, 1], "x":1.75, "y":3},
    {"matrix":[3, 2], "x":2.75, "y":3},
    {"matrix":[3, 3], "x":3.75, "y":3},
    {"matrix":[3, 4], "x":4.75, "y":3},
    {"matrix":[3, 5], "x":5.75, "y":3},
    {"matrix":[3, 6], "x":6.75, "y":3},
    {"matrix":[3, 7], "x":7.75, "y":3},
    {"matrix":[3, 8], "x":8.75, "y":3},
    {"matrix":[3, 9], "x":9.75, "y":3},
    {"matrix":[3,10], "x":10.75, "y":3},
    {"matrix":[3,11], "x":11.75, "y":3},
    {"matrix":[3,13], "x":12.75, "y":3, "w":2.25},
    {"matrix":[3,15], "x":15, "y":3},

    {"matrix":[4, 0], "x":0, "y":4, "w":2.25},
    {"matrix":[4, 2], "x":2.25, "y":4},
    {"matrix":[4, 3], "x":3.25, "y":4},
    {"matrix":[4, 4], "x":4.25, "y":4},
    {"matrix":[4, 5], "x":5.25, "y":4},
    {"matrix":[4, 6], "x":6.25, "y":4},
    {"matrix":[4, 7], "x":7.25, "y":4},
    {"matrix":[4, 8], "x":8.25, "y":4},
    {"matrix":[4, 9], "x":9.25, "y":4},
    {"matrix":[4,10], "x":10.25, "y":4},
    {"matrix":[4,11], "x":11.25, "y":4},
    {"matrix":[4,13], "x":12.25, "y":4, "w":1.75},
    {"matrix":[4,14], "x":14, "y":4},
    {"matrix":[4,15], "x":15, "y":4},

    {"matrix":[5, 0], "x":0, "y":5, "w":1.25},
    {"matrix":[5, 1], "x":1.25, "y":5, "w":1.25},
    {"matrix":[5, 2], "x":2.5, "y":5, "w":1.25},
    {"matrix":[5, 6], "x":3.75, "y":5, "w":6.25},
    {"matrix":[5,10], "x":10, "y":5},
    {"matrix":[5,11], "x":11, "y":5},
    {"matrix":[5,12], "x":12, "y":5},
    {"matrix":[5,13], "x":13, "y":5},
    {"matrix":[5,14], "x":14, "y":5},
    {"matrix":[5,15], "x":15, "y":5}
]"""

data = json.loads(data_str)

matrix = [[{'x': 0.0, 'y': 0.0, 'w': 0.0, 'h': 0.0} for r in range(16)] for c in range(6)] # [col][row]

for elem in data:
    c, r = elem['matrix'][0], elem['matrix'][1]
    matrix[c][r]['w'] = 1
    matrix[c][r]['h'] = 1
    matrix[c][r].update(elem)

yoffs = -0.5
for c in range(6):
    yoffs += matrix[c][0]['h'] / 2
    xoffs = -0.5
    for r in range(16):
        xoffs += matrix[c][r]['w'] / 2
        matrix[c][r]['ax'] = xoffs * 224./15.
        matrix[c][r]['ay'] = yoffs * 64./5.
        xoffs += matrix[c][r]['w'] / 2
    yoffs += matrix[c][0]['h'] / 2

print(matrix)

out = ""
for c in range(6):
    for r in range(16):
        if matrix[c][r]['w'] > 0:
            out += '{{{},{}}}, '.format(int(matrix[c][r]['ax']), int(matrix[c][r]['ay']))
    out += '\n'

print('\n')
# print(json.dumps(matrix, indent=4))
print(out)
print('\n')

data = [
    [  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 ],
    [ 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, -1, 30 ],
    [ 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, -1, 45 ],
    [ 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, -1, 58, -1, 59 ],
    [ 60, -1, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, -1, 71, 72, 73 ],
    [ 74, 75, 76, -1, -1, -1, 77, -1, -1, -1, 78, 79, 80, 81, 82, 83 ],
]

out = ""
for c, col in enumerate(data):
    out += "{"
    for r, index in enumerate(col):
        if index == -1:
            continue
        out += "{{{}, {}}}, ".format(int(r), int(c))
    out += "},\n"
print('\n', out, '\n')
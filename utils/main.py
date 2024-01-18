# This is a sample Python script.

# Press Ctrl+F5 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.


from svgpathtools import svg2paths, wsvg
import json


def read_svg(name_svg):
    # Чтение SVG
    # paths, attributes = svg2paths('map1.svg')
    paths, attributes = svg2paths(name_svg)

    holes = []
    for path in paths:  # iterate through all paths
        hole = []

        for path_obj in path:
            print(path_obj)
            entry = path_obj[0]
            print(entry)
            # print(entry.real)
            # print(entry.imag)
            point = [[] for x in range(2)]
            point[0] = entry.real
            point[1] = entry.imag
            hole += [point]

        holes += [hole]

    return holes


def print_holes(data):

    out_strings = []

    for element in data:
        out_string = json.dumps(element)
        out_strings += [out_string]

    with open("data_file.json", "w") as write_file:
        # json.dump(data, write_file)
        for out_string in out_strings:
            write_file.write(out_string)
            write_file.write("\n")





# TODO
# Функция преобразующая в нужный json [][][] и записыващая в файл map.json

def print_hi(name):
    # Use a breakpoint in the code line below to debug your script.
    print(f'Hi, {name}')  # Press F9 to toggle the breakpoint.

    #data = read_svg('map1.svg')
    data = read_svg('map10.svg')
    print_holes(data)



# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    print_hi('PyCharm')

# See PyCharm help at https://www.jetbrains.com/help/pycharm/

import subprocess


def problem(filename):
    data = subprocess.run(
        ['zbarimg', filename],
        stdout=subprocess.PIPE
    )
    data = data.stdout.decode('utf-8')
    data_list = [d.replace('QR-Code:', '') for d in data.splitlines()]
    total = 0
    head = []
    tail = None
    for d in data_list:
        num_str, shapes = d.split(':', 1)
        num = int(num_str)
        shapes_num = len(shapes.split(':'))
        total += num
        if num != shapes_num:
            tail = shapes
        else:
            head.append(shapes)
    head.append(tail)
    return '{}:{}'.format(total, ':'.join(head))


if __name__ == '__main__':
    import sys
    data = problem(sys.argv[1])
    with open('data/data.txt', 'w') as file:
        file.write(data)

#!/usr/bin/python

import random
import string
import sys
import getopt
from collections import Counter


def get_random_string(length):
    # choose from all lowercase letter
    letters = string.ascii_lowercase
    result_str = ''.join(random.choice(letters) for i in range(length))
    return result_str


def generate_random_test():
    test_list = []
    l = random.randint(1, 8192)
    for i in range(l):
        stringlen = random.randint(1, 30)
        randstring = get_random_string(stringlen)
        for i in range(random.randint(1, 16)):
            test_list.append(randstring)
    random.shuffle(test_list)
    req = []
    for i in range(random.randint(1, l)):
        req.append(random.choice(test_list))
    test_string = '{}\n{}\n{}\n{}\n{}'.format(len(req), len(
        ' '.join(test_list)), ' '.join(test_list), len(' '.join(req)), ' '.join(req))
    c = Counter(test_list)
    test = {}
    test['test'] = test_string
    ans_list = []
    for i in range(len(req)):
        ans_list.append('{}'.format(c[str(req[i])]))
    test['ans'] = ' '.join(ans_list)
    return test


def generate_n_tests(n):
    for i in range(n):
        text_file = open('tests/test{}.dat'.format(i), 'w+')

        test = generate_random_test()

        text_file.write(test['test'])
        text_file.close()

        text_file = open('tests/ans{}.dat'.format(i), 'w+')
        text_file.write(test['ans'])
        text_file.close()


def main(argv):
    try:
        opts, args = getopt.getopt(argv, 'hn:', ['num='])
    except getopt.GetoptError:
        print('gentest.py -n <number_of_tests>')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print('gentest.py -n <number_of_tests>')
            sys.exit()
        elif opt in ('-n', '--num'):
            generate_n_tests(int(arg))


if __name__ == "__main__":
    main(sys.argv[1:])

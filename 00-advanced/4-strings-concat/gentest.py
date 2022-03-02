#!/usr/bin/python

import random, string, sys, getopt, itertools, os

usage_string = "gentest.py -n <num> -o <output>"

lowercase = list(string.ascii_letters)


def GetRandomString(length):
    # choose from all lowercase letter
    random.shuffle(lowercase)
    return "".join(random.choice(lowercase) for i in range(length))


def GetRandomUnique(n, min, max):
    u = random.randint(min, max)
    while u == n:
        u = random.randint(min, max)
    return u


def GenerateRandomTest(matches, number):
    test_list = []
    for i in range(number):
        test_list.append(GetRandomString(random.randint(2, 256)))

    # Generate list of strings a + b = c + d
    for i in range(matches):
        stringlen = random.randint(24, 32)
        randstring = GetRandomString(stringlen)

        n1 = random.randint(2, stringlen - 2)
        split1 = [randstring[:n1], randstring[n1:]]

        n2 = GetRandomUnique(n1, 2, stringlen - 2)
        split2 = [randstring[:n2], randstring[n2:]]

        test_list.extend(split1)
        test_list.extend(split2)

    test_list = list(dict.fromkeys(test_list))
    random.shuffle(test_list)
    return test_list


def GenerateAns(test):
    map = {}
    for p in itertools.permutations(test, 2):
        pair_hash = hash("".join(p))
        if pair_hash in map:
            map[pair_hash].append(p)
        else:
            map[pair_hash] = [p]

    ans = []
    for key in map:
        if len(map[key]) > 1:
            d = {}
            for p in itertools.combinations(map[key], 2):
                new_key = "".join(["".join(p[0]), "".join(p[1])])
                if new_key in d:
                    if p not in d[new_key]:
                        d[new_key].append(p)
                else:
                    d[new_key] = [p]
            for k in d:
                temp = []
                temp.extend(d[k])
                ans.append(temp)

    return ans


class CmdArgs:
    number_of_tests = 0
    output_path = "./tests"
    number_of_strings = 64
    number_of_matches = 32

    def __init__(self):
        pass


def generate_n_tests(args):
    if not os.path.exists(args.output_path):
        os.mkdir(args.output_path)

    for i in range(args.number_of_tests):
        test = GenerateRandomTest(4, 128)

        file = open(os.path.join(args.output_path, "test{}.dat".format(i)), "w+")
        file.write("{}\n{}\n".format(len(test), " ".join(test)))
        file.close()

        ans = GenerateAns(test)
        file = open(os.path.join(args.output_path, "ans{}.dat".format(i)), "w+")

        n = 0
        ans_str = ""

        for i in ans:
            n += len(i)
            temp_str = ""
            for j in i:
                if "".join(j[0]) != "".join(j[1]):
                    print("Fatal error: {} != {}".format("".join(j[0]), "".join(j[1])))
                    sys.exit()
                temp_str += "{}\n".format(" ".join([" ".join(j[0]), " ".join(j[1])]))
            ans_str += "{}\n".format(temp_str)

        file.write("{}\n{}".format(n, ans_str))
        file.close()


def main(argv):
    cmd = CmdArgs()
    try:
        opts, args = getopt.getopt(argv, "hn:o:", ["num=", "output="])
    except getopt.GetoptError:
        print(usage_string)
        sys.exit()
    for opt, arg in opts:
        if opt == "-h":
            print(usage_string)
            sys.exit()
        if opt in ("-o", "--output"):
            cmd.output_path = str(arg)
        if opt in ("-n", "--num"):
            cmd.number_of_tests = int(arg)

    generate_n_tests(cmd)


if __name__ == "__main__":
    main(sys.argv[1:])

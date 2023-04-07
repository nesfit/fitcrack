import sys
import shutil

import os
from itertools import groupby


class Rules:
    def __init__(self, config):
        self.rule_types = ['Alpha', 'Digits', 'Other', 'Capitalization', 'Keyboard', 'Context']
        self.copy_dirs = ['Digits', 'Other', 'Keyboard', 'Context', 'Markov']

        self.config = config

        self.rulesets = dict()
        self.rulesets["Grammar"] = []
        self.rulesets["Capitalization"] = dict()
        self.rulesets["Alpha"] = dict()
        self.rulesets["Sizes"] = dict()


    def append_base_structure(self, rule):
        rule_arr = rule.split('\t')
        base = rule_arr[0]
        prob = float(rule_arr[1])
        tuple = base, prob
        self.rulesets["Grammar"].append(tuple)
        return 0

    # file 4.txt
    # rule ahoj 0.564
    # type "Alpha"
    def append_rule_from_file(self, file, rule, type):
        rule_arr = rule.split('\t')
        mask = rule_arr[0]
        prob = float(rule_arr[1])
        tuple = mask, prob
        self.rulesets[type][file].append(tuple)
        return 0

    def load_grammar(self):
        file_path = self.config.input_dir + '/Grammar/Grammar.txt'
        try:
            with open(file_path, 'r', encoding="ascii", errors="surrogateescape") as grammar_file:
                for rule in grammar_file:
                    self.append_base_structure(rule)

        except IOError:
            print("ERROR: File " + file_path + " cannot be opened", file=sys.stderr)
            return 1

        return 0

    def load_capitalization(self):
        dir_path = self.config.input_dir + '/Capitalization/'
        for file in os.listdir(dir_path):
            self.rulesets["Capitalization"][file] = []
            try:
                with open(dir_path + file, 'r', encoding="ascii", errors="surrogateescape") as f:
                    for rule in f:
                        self.append_rule_from_file(file, rule, "Capitalization")

            except IOError:
                print("ERROR: File " + dir_path + file + " cannot be opened", file=sys.stderr)
                return 1
        return 0

    def load_alpha(self):
        dir_path = self.config.input_dir + '/Alpha/'
        for file in os.listdir(dir_path):
            self.rulesets["Alpha"][file] = []
            try:
                with open(dir_path + file, encoding="ascii", errors="surrogateescape") as f:
                    for rule in f:
                        self.append_rule_from_file(file, rule, "Alpha")

            except IOError:
                print("ERROR: File " + dir_path + file + " cannot be opened", file=sys.stderr)
                return 1
        return 0

    def load_terminals_cnt(self):
        for rule_type in self.rule_types:
            self.rulesets["Sizes"][rule_type] = dict()
            dir_path = self.config.input_dir + '/' + rule_type + '/'
            for file in os.listdir(dir_path):
                try:
                    with open(dir_path + file, 'r', encoding="ascii", errors="surrogateescape") as f:
                        num_lines = sum(1 for line in f)
                        self.rulesets["Sizes"][rule_type][file] = num_lines

                except IOError:
                    print("ERROR: File " + dir_path + file + " cannot be opened", file=sys.stderr)
                    return 1

        return 0

    def get_file_size(self, value, size):
        rule_type = ""
        if value == "A":
            rule_type = "Alpha"
            return self.rulesets["Sizes"][rule_type][size + ".txt"] * self.rulesets["Sizes"]["Capitalization"][size + ".txt"]
        if value == "D":
            rule_type = "Digits"
        if value == "O":
            rule_type = "Other"
        if value == "K":
            rule_type = "Keyboard"
        if value == "X":
            rule_type = "Context"

        if rule_type == "":
            print("ERROR: get_file_size", file=sys.stderr)
            print("value: " + value, file=sys.stderr)
            print("size: " + str(size), file=sys.stderr)
            return -1

        return self.rulesets["Sizes"][rule_type][size + ".txt"]


    def get_guesses_cnt(self):
        cnt_all = 0
        for tuple in self.rulesets["Grammar"]:
            cnt_base = 1
            base_arr = [''.join(g) for _, g in groupby(tuple[0], str.isalpha)]
            for index in range(0, len(base_arr) // 2):
                value = base_arr[index * 2]
                size = base_arr[index * 2 + 1]

                file_size = self.get_file_size(value, size)
                if file_size == -1:
                    return -1
                cnt_base *= file_size

            cnt_all += cnt_base
        return cnt_all

    def write_grammar(self):
        os.mkdir(self.config.output_dir + "/Grammar")
        output_path = self.config.output_dir + '/Grammar/Grammar.txt'
        with open(output_path, 'w')  as g:
            for tuple in self.rulesets["Grammar"]:
                rule_string = tuple[0] + "\t" + str(tuple[1]) + "\n"
                g.write(rule_string)
        return 0

    def write_rule_type_dir(self, type):
        os.mkdir(self.config.output_dir + "/" + type)
        for file in self.rulesets[type]:
            with open(self.config.output_dir + "/" + type + "/" + file, 'w')  as c:
                for tuple in self.rulesets[type][file]:
                    rule_string = tuple[0] + "\t" + str(tuple[1]) + "\n"
                    c.write(rule_string)

        return 0

    def save_new_grammar(self):
        if os.path.isdir(self.config.output_dir):
            shutil.rmtree(self.config.output_dir)
        os.mkdir (self.config.output_dir)

        for dirname in self.copy_dirs:
            original_dir = self.config.input_dir + '/' + dirname
            target_dir = self.config.output_dir + '/' + dirname
            shutil.copytree(original_dir, target_dir)

        shutil.copy(self.config.input_dir + '/config.ini', self.config.output_dir + '/')

        self.write_grammar()
        self.write_rule_type_dir("Capitalization")
        self.write_rule_type_dir("Alpha")
        return 0

    def append_attack_dictionaries(self, ad):
        for file in ad.dictionaries.keys():
            for len in ad.dictionaries[file].keys():
                alpha_file = str(len) + ".txt"

                if alpha_file not in self.rulesets["Alpha"]:
                    #print("skipping length " + str(len) + "...")
                    continue
                # create dict from list
                tmp_dic = dict(self.rulesets["Alpha"][alpha_file])
                # add new words from attack dictionaries
                # dict solves duplicities
                # if word already exists in grammar and has smaller prob then overwrite it
                # password  0.00123 => password  0.0238
                for word, prob in ad.dictionaries[file][len].items():
                    if word in tmp_dic:
                        if tmp_dic[word] < prob:
                            tmp_dic[word] = prob
                            ad.successfully_appended[file][word] = prob
                            ad.duplicities[file][word] = prob
                    else:
                        tmp_dic[word] = prob
                        ad.successfully_appended[file][word] = prob
                # convert dict back to sorted list
                sorted_list = sorted(tmp_dic.items(), key=lambda kv: kv[1], reverse=True)
                # update list
                self.rulesets["Alpha"][alpha_file] = sorted_list
        return 0

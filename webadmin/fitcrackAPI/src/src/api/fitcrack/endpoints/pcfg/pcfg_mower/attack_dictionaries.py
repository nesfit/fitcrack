import sys
import math
import random
import logging
from collections import defaultdict

from debug import Debug

class Attack_dictionaries:
    def __init__(self, config):
        self.config_file = config
        self.priorities = dict()
        self.dictionaries = dict()
        self.successfully_appended = defaultdict(dict)
        self.duplicities = defaultdict(dict)

        # priorities["file"] = "high"
        # dictionaries["file"]["length"]["password"] = 0.1344

    def load_dictionaries(self):
        try:
            with open(self.config_file, 'r') as conf_file:
                for line in conf_file:
                    line_arr = line.split('\t')
                    dict_file = line_arr[0]
                    dict_priority = line_arr[1].rstrip()
                    self.priorities[dict_file] = dict_priority
                    self.append_dictionary(dict_file)

        except IOError:
            print("ERROR: File " + self.config_file + " cannot be opened", file=sys.stderr)
            return 1
        return 0

    def remove_words_with_len(self, file, len):
        self.dictionaries[file].pop(len)
        return 0

    def append_dictionary(self, dict_file):
        self.dictionaries[dict_file] = defaultdict(dict)
        try:
            with open(dict_file, 'r') as f:
                for word in f:
                    word = word.rstrip()
                    word_len = len(word)
                    self.dictionaries[dict_file][word_len][word] = 0.0

        except IOError:
            print("ERROR: File " + dict_file + " cannot be opened", file=sys.stderr)
            return 1
        return 0

    def analyse_alpha_file_prob(self, percentage, grammar_alpha_file, rules):
        # first item in An.txt (password 0.1254546)
        _, top_prob = rules.rulesets["Alpha"][grammar_alpha_file][0]
        words_cnt = rules.rulesets["Sizes"]["Alpha"][grammar_alpha_file]
        lowest_prob_index = words_cnt / 100 * percentage
        lowest_prob_index = int(math.ceil(lowest_prob_index))
        if lowest_prob_index >= words_cnt:
            lowest_prob_index = words_cnt-1
        _, lowest_prob = rules.rulesets["Alpha"][grammar_alpha_file][lowest_prob_index]
        #debug = Debug()
        #debug.print_analysed_alpha_file(grammar_alpha_file, top_prob, lowest_prob, lowest_prob_index, words_cnt)
        return (top_prob, lowest_prob)

    def assign_probability(self, rules):
        for file in self.dictionaries.keys():
            priority = self.priorities[file]
            if priority == "high":
                #percentage = 5
                percentage = 3
            elif priority == "medium":
                #percentage = 30
                percentage = 20
            elif priority == "low":
                #percentage = 60
                percentage = 40
            else:
                print("ERROR: undefined priority" + priority, file=sys.stderr)
                return 1
            for len in self.dictionaries[file].keys():
                # Does file exist in grammar?
                alpha_file = str(len) + ".txt"
                if alpha_file not in rules.rulesets["Alpha"]:
                    #print("You are trying to add new word to Alpha[" + alpha_file + "] but the file doesn't exist")
                    continue
                top_prob, low_prob = self.analyse_alpha_file_prob(percentage, alpha_file, rules)
                for word in self.dictionaries[file][len].keys():
                    #generated_prob = random.uniform(low_prob, top_prob)
                    #generated_prob = round(generated_prob,8)
                    self.dictionaries[file][len][word] = low_prob
        return 0



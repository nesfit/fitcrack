#!/usr/bin/env python3

from __future__ import print_function
import sys
import argparse
import logging

from attack_dictionaries import Attack_dictionaries
from config import Config
from debug import Debug
from rules import Rules
from filter import Filter

if sys.version_info[0] < 3:
    print("This program requires Python 3.x", file=sys.stderr)
    sys.exit(1)

def parse_arguments(config):
    parser = argparse.ArgumentParser(description='Filters ruleset by given settings')
    parser.add_argument('--input','-i', help='Input grammar directory', required=True)
    parser.add_argument('--output','-o', help='Output grammar directory', required=False)
    parser.add_argument('--limit', '-l', help='Password guesses limit', required=False)
    parser.add_argument('--attackdictionaries', '-a', help='Config files for attack dictionaries', required=False)

    try:
        args=parser.parse_args()
        config.input_dir = args.input
        if args.output:
            config.output_dir = args.output
        if args.limit:
            config.limit = int(args.limit)
        if args.attackdictionaries:
            config.attack_dict_file = args.attackdictionaries

    except Exception as msg:
        print(msg)
        return 1

    return 0

def print_ruleset_stat(rules, filter, cs, guess_cnt, quiet):
    if quiet:
        return 0
    print(" Guess count: " + str(guess_cnt))
    print(" Grammar prob: " + str(filter.get_total_grammar_prob()))
    print(" Grammar size: " + str(len(rules.rulesets["Grammar"])))
    print(" Capitalization cs: " + str(cs))
    cap_rules = 0
    cap_prob = 0.0
    for file in rules.rulesets["Capitalization"]:
        cap_rules += len(rules.rulesets["Capitalization"][file])
        for tuple in rules.rulesets["Capitalization"][file]:
            cap_prob += tuple[1]
    print(" Capitalization rules: " + str(cap_rules))
    print(" Capitalization files: " + str(len(rules.rulesets["Capitalization"])))
    print(" Capitalization prob: " + str(cap_prob))
    alpha_size = 0
    for file in rules.rulesets["Sizes"]["Alpha"]:
        alpha_size += rules.rulesets["Sizes"]["Alpha"][file]
    print(" Alpha size: " + str(alpha_size))
    print("")
    return 0

def pcfg_mower(config):
    print = logging.info
    iprint = logging.warning
    logging.basicConfig(level=logging.WARNING if config.quiet else logging.INFO,
                        format="%(message)s")

    rules = Rules(config)
    debug = Debug()

    if rules.load_grammar():
        iprint("ERROR: load_grammar", file=sys.stderr)
        return 1

    if rules.load_capitalization():
        iprint("ERROR: load_capitalization", file=sys.stderr)
        return 1

    if rules.load_alpha():
        iprint("ERROR: load_alpha", file=sys.stderr)
        return 1

    if rules.load_terminals_cnt():
        iprint("ERROR: load_terminals_cnt", file=sys.stderr)
        return 1

    guess_cnt = rules.get_guesses_cnt()
    if guess_cnt == -1:
        iprint("ERROR: get_guesses_cnt", file=sys.stderr)
        return 1
    #print("Original:\t" + str(guess_cnt))
    #print(str(guess_cnt))
    sys.stdout.write(str(guess_cnt))

    if config.output_dir == "":
        # output grammar is not defined
        # just printing password guesses count
        # debug.print_prob_groups(rules)
        return 0

    filter = Filter(rules)

    print("limit: " + str(config.limit))
    print("bs: " + str(config.bs))
    print("cs: " + str(config.cs))
    print("")

    print(config.input_dir)
    cs = config.cs
    print_ruleset_stat(rules, filter, cs, guess_cnt, config.quiet)

    if not config.attack_dict_file == "":
        attack_dictionaries = Attack_dictionaries(config.attack_dict_file)
        if attack_dictionaries.load_dictionaries():
            iprint("ERROR: load_dictionaries", file=sys.stderr)
            return 1
        if attack_dictionaries.assign_probability(rules):
            iprint("ERROR: assign_probability", file=sys.stderr)
            return 1
        rules.append_attack_dictionaries(attack_dictionaries)
        filter.rebuild_size("Alpha")
        guess_cnt = rules.get_guesses_cnt()
        if guess_cnt == -1:
            iprint("ERROR: get_guesses_cnt", file=sys.stderr)
            return 1

        debug.print_appended_dictionary_words(attack_dictionaries, config.quiet)

    if config.limit == 0:
        print(config.output_dir)
        print_ruleset_stat(rules, filter, cs, guess_cnt, config.quiet)
        rules.save_new_grammar()
        return 0

    while (guess_cnt > config.limit):
        filter.mow_grammar()
        filter.mow_capitalization(cs)
        guess_cnt = rules.get_guesses_cnt()
        cs += config.cs

    print(config.output_dir)
    print_ruleset_stat(rules, filter, cs, guess_cnt, config.quiet)

    if rules.save_new_grammar():
        iprint("ERROR: save_new_grammar", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    config = Config

    if parse_arguments(config):
        print("ERROR: parse_arguments", file=sys.stderr)
        exit(1)

    pcfg_mower(config)

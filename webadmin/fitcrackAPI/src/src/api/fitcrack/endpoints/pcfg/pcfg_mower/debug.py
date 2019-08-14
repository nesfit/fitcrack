from collections import defaultdict

class Debug:
    def print_ruleset(self, rules):
        for type in rules.rulesets.keys():
            print(" " + type)
            for file in rules.rulesets[type].keys():
                size = len(rules.rulesets[type][file])
                print("   [" + file + "]   size: " + str(size))
                print(rules.rulesets[type][file])
        return 0

    def print_ruleset_type(rules, type):
        print(" " + type)
        for file in rules.rulesets[type].keys():
            size = len(rules.rulesets[type][file])
            print("   [" + file + "]   size: " + str(size))
            print(rules.rulesets[type][file])
        return 0

    def print_ruleset_type_file(rules, type, file):
        print(" " + type)
        size = len(rules.rulesets[type][file])
        print("   [" + file + "]   size: " + str(size))
        i = 1
        for tuple in rules.rulesets[type][file]:
            print("     " + str(i) + ": " + str(tuple))
            i += 1
        return 0

    def print_dictionaries(self, ad):
        print(ad.priorities)
        for file in ad.dictionaries.keys():
            print(file)
            for len in ad.dictionaries[file].keys():
                print("   [" + str(len) + "] ")
                for word, prob in ad.dictionaries[file][len].items():
                    print("       " + str(word) + "  " + str(prob))

        return 0

    def print_analysed_alpha_file(self, grammar_alpha_file, top_prob, lowest_prob, lowest_prob_index, words_cnt):
        print("  [" + grammar_alpha_file + "]")
        print("      top[0]: " + str(top_prob))
        print("      low[" + str(lowest_prob_index) + "]: " + str(lowest_prob))
        print("      size: " + str(words_cnt))
        return 0

    def print_appended_dictionary_words(self, ad, quiet):
        if quiet:
            return 0
        missing = defaultdict(dict)
        all = defaultdict(dict)
        for file in ad.dictionaries.keys():
            for lenn in ad.dictionaries[file].keys():
                for word, prob in ad.dictionaries[file][lenn].items():
                    all[file][word] = prob

        for file in all.keys():
            for word, prob in all[file].items():
                if word not in ad.successfully_appended[file]:
                    missing[file][word] = prob

        print()
        print("APPENDED")
        for file in ad.successfully_appended.keys():
            cnt = len(ad.successfully_appended[file])
            #print()
            print("  " + file + ": " + str(cnt))
            #for word, prob in ad.successfully_appended[file].items():
                #print("    " + word + "\t" + str(prob))

        print()
        print("DUPLICITIES")
        for file in ad.duplicities.keys():
            cnt = len(ad.duplicities[file])
            #print()
            print("  " + file + ": " + str(cnt))
            #for word, prob in ad.duplicities[file].items():
                #print("    " + word + "\t" + str(prob))

        print()
        print("MISSING")
        for file in missing.keys():
            cnt = len(missing[file])
            #   print()
            print("  " + file + ": " + str(cnt))
            #for word, prob in missing[file].items():
                #print("    " + word + "\t" + str(prob))

        print()

    def print_prob_groups(self, rules):
        total = 0
        known_probs = defaultdict(dict)
        for file in rules.rulesets["Alpha"].keys():
            for tuple in rules.rulesets["Alpha"][file]:
                known_probs[file][tuple[1]] = True

        for file in known_probs.keys():
            file_cnt = len(known_probs[file])
            total += file_cnt
            print(file + ":\t" + str(file_cnt))

        print("Total prob groups: " + str(total))
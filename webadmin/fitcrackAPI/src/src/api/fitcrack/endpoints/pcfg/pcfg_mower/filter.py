class Filter:
    def __init__(self, rules):
        self.rules = rules

    def get_total_grammar_prob(self):
        total_prob = 0
        for tuple in self.rules.rulesets["Grammar"]:
            total_prob += tuple[1]
        return total_prob

    # type = "Capitalization" | "Alpha"
    def rebuild_size(self, type):
        self.rules.rulesets["Sizes"][type].clear()

        for file in self.rules.rulesets[type]:
            num_lines = len(self.rules.rulesets[type][file])
            self.rules.rulesets["Sizes"][type][file] = num_lines

        return 0

    def mow_grammar(self):
        prob_before_mowing = self.get_total_grammar_prob()
        prob_diff = 0

        while (prob_diff < self.rules.config.bs):
            grammar_len = len(self.rules.rulesets["Grammar"])
            self.rules.rulesets["Grammar"].pop(grammar_len-1)
            prob_after_mowing = self.get_total_grammar_prob()
            prob_diff = prob_before_mowing - prob_after_mowing

        return 0

    def mow_capitalization(self, cs):
        removed = False
        for file in self.rules.rulesets["Capitalization"]:
            for tuple in self.rules.rulesets["Capitalization"][file]:
                if tuple[1] < cs:
                    self.rules.rulesets["Capitalization"][file].remove(tuple)
                    removed = True
        if removed:
            self.rebuild_size("Capitalization")

        return 0

## PCFG Mower

### Overview
PCFG Mower is a submodule of PCFG Cracker tool (https://github.com/lakiw/pcfg_cracker) which mainly analyzes and filters input grammar. Program counts how many passwords could be created by input grammar and filters it till password count falls below certain limit. Filtering is done by removing the least probable base structures and capitalization rules.

### Parameters
--input, -i&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Input Grammar created with pcfg_trainer  
--output, -o&nbsp;&nbsp;Filtered Grammar  
--limit, -l&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Password guesses count limit  

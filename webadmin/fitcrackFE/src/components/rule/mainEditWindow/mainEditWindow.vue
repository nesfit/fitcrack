<template>
    <v-col cols="12" md="8" class="bordered">
        <v-container>
            <v-row justify="center" class="border-down text-h5 py-2 rounded">
                Create rule file
            </v-row>
            <v-row>
                <v-col cols="6">
                    <v-text-field label="Rule file name" required outlined autofocus
                        hint="Give this rule file a name (.txt or .rule extension)" persistent-hint></v-text-field>
                </v-col>
                <v-spacer></v-spacer>
                <v-col>
                    <ruleFunctions></ruleFunctions>
                </v-col>

            </v-row>

            <v-row justify-start align-start>
                <v-col>
                    <v-btn class="px-2" color="orange lighten-3" depressed @click="addEmptyRule">
                        <v-icon left>
                            mdi-plus
                        </v-icon>
                        Add rule
                    </v-btn>
                </v-col>
                <v-spacer></v-spacer>
                <v-col>
                    <v-btn class="px-2" color="orange lighten-3" depressed @click="generateRandomRule">
                        <v-icon>
                            mdi-plus-thick
                        </v-icon>
                        Add random rule
                    </v-btn>
                </v-col>
                <v-spacer></v-spacer>
                <v-col>
                    <input type="file" ref="appendRuleFile" style="display: none" @change="onRuleFileChange($event)">
                    <v-btn class="px-2" color="orange lighten-3" depressed @click="$refs.appendRuleFile.click()">
                        <v-icon left>
                            mdi-file
                        </v-icon>
                        Append rule file
                    </v-btn>
                </v-col>
                <v-spacer></v-spacer>

                <v-col>
                    <v-btn class="px-2" color="red accent-2" depressed @click="resetRules">
                        <v-icon>
                            mdi-delete
                        </v-icon>
                        Reset rules</v-btn>
                </v-col>
            </v-row>
            <v-row>
                <ruleFileContent v-bind:rulesList="rulesList" v-on:rules-updated="updateRules"></ruleFileContent>
            </v-row>
            <v-row>
                <v-col align="right">
                    Rule count: {{ ruleCount }}
                </v-col>

            </v-row>

            <v-row>
                <v-col class="text-right">
                    <v-btn color="grey lighten-1" depressed>
                        <v-icon>
                            mdi-content-save
                        </v-icon>
                        Save file</v-btn>
                </v-col>
            </v-row>
        </v-container>
    </v-col>
</template>


<script>
import ruleFileContent from '@/components/rule/mainEditWindow/ruleFileContent.vue';
import ruleFunctions from '@/components/rule/mainEditWindow/popups/ruleFunctions.vue';
export default {
    props: ["rulesList"],
    data() {
        return {
            minFunctionsNum: 6,
            maxFunctionsNum: 8,
            randomRuleString: ""
        };
    },
    methods: {
        onRuleFileChange(event) {
            const file = event.target.files[0];
            const reader = new FileReader();
            reader.onload = (event) => {
                this.rulesContent = event.target.result;
                this.rulesList = this.rulesList.concat(event.target.result.split("\n"));
                this.rulesList.pop();
                this.$emit("rules-updated", this.rulesList)
            };
            reader.readAsText(file);
        },
        generateRandomRule() {
            const data = {
                minFunctionsNum: this.minFunctionsNum,
                maxFunctionsNum: this.maxFunctionsNum
            };
            this.axios.post(this.$serverAddr + "/rule/randomRule", data).then((response) => {
                this.randomRuleString = response.data.randomRule;
                this.rulesList.push(this.randomRuleString);
            }).catch((error) => {
                this.randomRuleString = error.message;
            });
        },
        addEmptyRule() {
            this.rulesList.push("")
        },
        updateRules(updatedRulesList) {
            this.rulesList = updatedRulesList;
            console.log("updatedParent")
            this.$emit("rules-updated", this.rulesList)
        },
        resetRules() {
            this.rulesList = []
            this.updateRules(this.rulesList)
        }
    },
    computed: {
        ruleCount() {
            return this.rulesList.length;
        }
    },
    components: {
        ruleFileContent,
        ruleFunctions
    },



};

</script>













<style>
.bordered {
    border: 2px solid darkgray;
    border-top-right-radius: 4px;
    border-bottom-right-radius: 4px;
    border-top-left-radius: 4px;
    border-bottom-left-radius: 4px;
    margin: 4px;
    margin-top: 10px;
}

.border-down {
    border-bottom: 1px solid darkgray;
}
</style>
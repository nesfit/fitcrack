<!--
   * Author : Jiri Mladek
   * Licence: MIT, see LICENSE
-->

<template>
    <v-col cols="12" md="8" class="bordered">
        <v-sheet outlined color="grey lighten-1" rounded>
            <v-card>
                <v-card-title>
                    <v-row justify="center" class="border-down text-h5 font-weight-medium py-2 rounded">
                        {{ editingFile ? "Edit rule file" : "Create rule file" }}
                    </v-row>
                </v-card-title>
                <v-container>
                    <v-row>
                        <v-col sm="7" cols="12" class="pb-0">
                            <v-text-field v-model="ruleFileInfo.name" label="Rule file name" required outlined autofocus
                                hint="Give this rule file a name (.txt or .rule extension)" persistent-hint></v-text-field>
                        </v-col>
                        <v-spacer></v-spacer>
                        <v-col>
                            <v-btn color="custom_grey" @click="showAllFunctionsPopup({ visible: true, onlyShow: true })">
                                Show rule functions
                            </v-btn>
                        </v-col>
                    </v-row>
                    <v-row>
                        <v-col>
                            <v-btn class="px-2 mr-3 mb-2" color="orange lighten-3 black--text"  @click="addEmptyRule">
                                <v-icon left>
                                    mdi-plus
                                </v-icon>
                                Add rule
                            </v-btn>

                            <v-btn class="px-2 mr-3 mb-2" color="orange lighten-3 black--text" @click="generateRandomRule">
                                <v-icon>
                                    mdi-plus
                                </v-icon>
                                Add random rule
                            </v-btn>

                            <v-btn class="px-2 mr-3 mb-2 orange lighten-3 black--text " color="primary" 
                                @click="appendRuleFilePopup = true">
                                <v-icon left>
                                    mdi-file
                                </v-icon>
                                Append rule file
                            </v-btn>
                            <appendRulePopup v-model="appendRuleFilePopup" v-bind:rules="rules"
                                v-on:update-rules="updateRules">
                            </appendRulePopup>

                            <v-btn class="px-2 mr-3 mb-2" color="red accent-2 black--text"  @click="resetRules">
                                <v-icon>
                                    mdi-delete
                                </v-icon>
                                Reset rules</v-btn>
                        </v-col>
                    </v-row>
                    <v-row class="mt-0">
                        <ruleFileContent v-bind:rules="rules" v-on:update-rules="updateRules"
                            v-on:show-insert-popup="showInsertPopup" v-on:show-all-functions-popup="showAllFunctionsPopup">
                        </ruleFileContent>
                    </v-row>
                    <v-row>
                        <v-col align="right" class="pr-4">
                            Rules count: <b>{{ rulesCount }}</b>
                        </v-col>
                    </v-row>
                    <v-row>
                        <v-col class="text-right">
                            <v-btn color="primary black--text" @click="saveFile()">
                                <v-icon>
                                    mdi-content-save
                                </v-icon>
                                {{ !editingFile ? "Save file" : "Update File" }}
                            </v-btn>
                        </v-col>
                    </v-row>
                </v-container>
            </v-card>
        </v-sheet>
    </v-col>
</template>


<script>
import ruleFileContent from '@/components/rule/mainEditWindow/ruleFileContent.vue';
import appendRulePopup from '@/components/rule/mainEditWindow/popups/appendRulePopup.vue';

export default {
    props: {
        rules: Array, // array of rules {value, error}
        rulesCount: Number, // number of rules in a rule file
        editingFile: Boolean, // flag to indicate if file is being created or edited
        ruleFileInfo: Object // information about rule file got from server
    },
    data() {
        return {
            appendRuleFilePopup: false, // boolean indicating if append rule file popup should be shown
            ruleFile: null // variable for storing the newly created rule file, will be sent on the server when saved
        };
    },
    methods: {
        /**
         * Method which gets generated random rule from server
         */
        generateRandomRule() {
            this.axios.get(this.$serverAddr + "/rule/randomRule").then((response) => {
                const randomRule = { value: response.data.randomRule, error: false }; // save the value of generated random rule
                let updatedRules = this.rules;
                updatedRules.push(randomRule); // add to existing rules
                this.$emit("update-rules", updatedRules) // update in parent
            });
        },
        /**
         * Method which adds a new empty rule to existing ones 
         */
        addEmptyRule() {
            let updatedRules = this.rules;
            updatedRules.push({ value: "", error: false })
            this.$emit("update-rules", updatedRules, true) // update in parent
        },
        /**
         * Method which emits updating rules in parent
         * @param {Array} updatedRules Updated rules
         */
        updateRules(updatedRules) {
            this.$emit("update-rules", updatedRules)
        },
        /**
         * Method which deletes all rules
         */
        resetRules() {
            this.$emit("update-rules", [{ value: "", error: false }])
        },
        /**
         * Method which propagates showing insert popup to parent
         * @param {Object} popupData Data for Insert popup to be updated 
         */
        showInsertPopup(popupData) {
            this.$emit("show-insert-popup", popupData)
        },
        /**
         * Method which propagates showing all functions popup to parent
         * @param {Object} popupData Data for All functions popup to be updated 
         */
        showAllFunctionsPopup(popupData) {
            this.$emit("show-all-functions-popup", popupData)
        },
        /**
         * Method which converts rules into a file
         */
        convertRulesToFile() {
            // create a string from the rules values
            const rulesArray = this.rules.map(rule => rule.value);
            const rulesString = rulesArray.join('\n');
            // create a file with rules content
            const blob = new Blob([rulesString], { type: 'text/plain' });
            const file = new File([blob], this.ruleFileInfo.name, { type: 'text/plain' });
            this.ruleFile = file;
        },
        /**
         * Method which saves the file (when creating -> creates as new, when editing -> updates current rule file)
         */
        saveFile() {
            this.convertRulesToFile();
            const formData = new FormData();
            formData.append('file', this.ruleFile);
            var config = {
                withCredentials: true,
                headers: {
                    'Content-type': 'multipart/form-data'
                },
            }
            // when creating rule file, send POST request
            if (!this.editingFile) {
                // upload the file to server
                this.axios.post(this.$serverAddr + "/rule", formData, config).then((response) => {
                    this.file = null
                    this.$router.push({ name: 'rules', params: { skipConfirmWindow: true } });
                }).catch(error => {
                    console.log(error)
                });
            }
            // when editing rule, send PUT request
            else {
                // upload the updated file to server
                this.axios.put(this.$serverAddr + "/rule/" + this.$route.params.id, formData, config).then((response) => {
                    this.file = null
                    this.$router.push({ name: 'rules', params: { skipConfirmWindow: true } });
                }).catch(error => {
                    console.log(error)
                });
            }

        }
    },
    components: {
        ruleFileContent,
        appendRulePopup
    },
};

</script>




<style>
.border-down {
    border-bottom: 3px solid #BDBDBD;
}
</style>
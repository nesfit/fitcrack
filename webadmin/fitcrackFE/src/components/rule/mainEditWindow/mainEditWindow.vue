<template>
    <v-col cols="12" md="8" class="bordered">
        <v-container>
            <v-row justify="center" class="border-down text-h5 py-2 rounded">
                <div v-if="!editingFile">
                    Create rule file
                </div>
                <div v-else>
                    Edit rule file
                </div>

            </v-row>
            <v-row>
                <v-col cols="6">
                    <v-text-field v-model="ruleFileInfo.name" label="Rule file name" required outlined autofocus
                        hint="Give this rule file a name (.txt or .rule extension)" persistent-hint></v-text-field>
                </v-col>
                <v-spacer></v-spacer>
                <v-col>
                    <v-btn color="grey lighten-1" @click="showAllFunctionsPopup({ visible: true, onlyShow: true })">
                        Show rule functions
                    </v-btn>
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
                    <v-btn class="px-2" color="orange lighten-3" depressed @click="appendRuleFilePopup = true">
                        <v-icon left>
                            mdi-file
                        </v-icon>
                        Append rule file
                    </v-btn>
                    <appendRulePopup v-model="appendRuleFilePopup" v-bind:rules="rules" v-on:update-rules="updateRules">
                    </appendRulePopup>
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
                <ruleFileContent v-bind:rules="rules" v-on:update-rules="updateRules"
                    v-on:show-insert-popup="showInsertPopup" v-on:show-all-functions-popup="showAllFunctionsPopup">
                </ruleFileContent>
            </v-row>
            <v-row>
                <v-col align="right">
                    Lines count: {{ ruleCount }}
                </v-col>

            </v-row>

            <v-row>
                <v-col class="text-right">
                    <v-btn class="orange darken-3" @click="saveFile()">
                        <v-icon>
                            mdi-content-save
                        </v-icon>
                        {{ !editingFile ? "Save file" : "Update File" }}
                    </v-btn>
                </v-col>
            </v-row>
        </v-container>
    </v-col>
</template>


<script>
import ruleFileContent from '@/components/rule/mainEditWindow/ruleFileContent.vue';
import appendRulePopup from '@/components/rule/mainEditWindow/popups/appendRulePopup.vue';
export default {
    props: {
        rules: Array,
        editingFile: Boolean,
        ruleFileInfo: Object
    },
    data() {
        return {
            appendRuleFilePopup: false,
            ruleFile: null
        };
    },
    methods: {
        generateRandomRule() {
            this.axios.get(this.$serverAddr + "/rule/randomRule").then((response) => {
                const randomRule = { value: response.data.randomRule, error: false };
                let updatedRules = this.rules; //copy 
                updatedRules.push(randomRule);
                this.$emit("update-rules", updatedRules)
            });
        },
        addEmptyRule() {
            let updatedRules = this.rules;
            updatedRules.push({value: "", error: false})
            this.$emit("update-rules", updatedRules, true)
        },
        updateRules(updatedRules) {
            this.$emit("update-rules", updatedRules)
        },
        resetRules() {
            this.$emit("update-rules", [{value: "", error: false}])
        },
        updateFunctionsPopupState(updatedState) {
            this.showFunctionsPopup = updatedState;
        },
        showInsertPopup(insertData) {
            this.$emit("show-insert-popup", insertData)
        },
        showAllFunctionsPopup(popupData) {
            this.$emit("show-all-functions-popup", popupData)
        },
        /**
         * Function which converts rules into a file
         */
        convertRulesToFile() {
            //create a string from the rules values
            const rulesArray = this.rules.map(rule => rule.value);
            const rulesString = rulesArray.join('\n');
            const blob = new Blob([rulesString], { type: 'text/plain' });
            const file = new File([blob], this.ruleFileInfo.name, { type: 'text/plain' });
            this.ruleFile = file;
        },
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
            if (!this.editingFile) {
                //upload the file to server
                this.axios.post(this.$serverAddr + "/rule", formData, config).then((response) => {
                    this.file = null
                    this.$router.push({ name: 'rules', params: { skipConfirmWindow: true } });
                }).catch(error => {
                    console.log(error)
                });
            }
            else {
                this.axios.put(this.$serverAddr + "/rule/" + this.$route.params.id, formData, config).then((response) => {
                    this.file = null
                    this.$router.push({ name: 'rules', params: { skipConfirmWindow: true } });
                }).catch(error => {
                    console.log(error)
                });
            }

        }
    },
    computed: {
        ruleCount() {
            return this.rules.length;
        }
    },
    components: {
        ruleFileContent,
        appendRulePopup
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
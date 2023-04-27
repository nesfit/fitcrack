<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
    <v-dialog v-model="popupVisible">
        <v-card>
            <v-card-title class="headline orange lighten-3">
                Append rule file
            </v-card-title>
            <v-tabs v-model="tab">
                <v-tab>
                    From filesystem
                </v-tab>
                <v-tab>
                    From server
                </v-tab>
            </v-tabs>
            <v-tabs-items v-model="tab">
                <v-tab-item>
                    <v-card text class="text-center py-2">
                        <v-file-input accept=".txt, .rule" outlined chips show-size class="pr-2" label="Select file"
                            @change="onRuleFileChange" />
                        <v-btn color="primary" outlined :disabled="systemFileSelected === false" @click="appendRules">
                            Append
                            <v-icon right>
                                mdi-plus
                            </v-icon>
                        </v-btn>
                    </v-card>
                </v-tab-item>
                <v-tab-item>
                    <v-card text class="text-center py-2">
                        <rulesSelector v-model="serverFileSelected"></rulesSelector>
                        <v-btn color="primary" outlined :disabled="serverFileSelected.length == 0"
                            @click="appendServerRules">
                            Append
                            <v-icon right>
                                mdi-plus
                            </v-icon>
                        </v-btn>

                    </v-card>

                </v-tab-item>
            </v-tabs-items>
        </v-card>
    </v-dialog>
</template>
  
    
<script>
import rulesSelector from '@/components/selector/rulesSelector.vue'

export default {
    props: {
        value: Boolean, // for visibility of popup
        rules: Array // array of rules {value, error}
    },
    data() {
        return {
            updatedRules: [], // array for storing rules after append
            tab: null, // variable for v-tab choice
            systemFileSelected: false, // boolean to indicate that file from filesystem was selected
            serverFileSelected: [] // array to store selected file from server
        };
    },
    methods: {
        /**
         * Method which gets the content of selected rule file from filesystem and concatenates it with existing rules
         * @param {*} event Selected file from filesystem 
         */
        onRuleFileChange(event) {
            // if file is selected
            if (event) {
                const reader = new FileReader();
                reader.onload = (event) => {
                    const lines = reader.result.split("\n"); // get rules from the selected file
                    const newRules = lines.map(line => ({ value: line, error: false }))
                    this.updatedRules = this.rules.concat(newRules); // concatenate rules from new file with existing ones
                    this.updatedRules.pop(); // delete the last line
                };
                reader.readAsText(event)
                this.systemFileSelected = true;
            }
            else {
                this.systemFileSelected = false;
            }
        },
        /**
         * Method which appends the rule file (from client filesystem) after clicking the button
         */
        appendRules() {
            this.$emit("update-rules", this.updatedRules) // update rules in parent
            this.systemFileSelected = false;
            this.popupVisible = false;
        },
        /**
         * Method which appends the content of selected rule file (from server)
         */
        appendServerRules() {
            // get content of the selected rule file
            this.axios.get(this.$serverAddr + "/rule/" + this.serverFileSelected[0].id + "/download").then((response) => {
                const lines = response.data.split("\n");
                const newRules = lines.map(line => ({ value: line, error: false }))
                this.updatedRules = this.rules.concat(newRules); // concatenate rules from new file with existing ones
                this.updatedRules.pop(); // delete the last line
                this.$emit("update-rules", this.updatedRules) // update rules in parent
                this.serverFileSelected = []
                this.popupVisible = false;
            });
        }
    },
    computed: {
        // getter and setter for visibility of popup
        popupVisible: {
            get() {
                return this.value;
            },
            set(visibility) {
                this.$emit('input', visibility);
            }
        }
    },
    components: {
        rulesSelector
    }

};
</script>
  
  
<style></style>
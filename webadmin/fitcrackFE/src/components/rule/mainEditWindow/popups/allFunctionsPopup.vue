<!--
   * Author : Jiri Mladek
   * Licence: MIT, see LICENSE
-->

<template>
    <v-dialog v-model="allFunctionsPopup.visible" max-width="1200">
        <v-card>
            <v-card-title justify-center class="headline primary">
                {{ allFunctionsPopup.onlyShow ? "Rule functions" : "Choose a rule function" }}
                <v-spacer></v-spacer>
                <v-card-actions>
                    <v-btn class="close-btn" color="grey darken-4" fab x-small dark @click="hidePopup()">
                        <v-icon>mdi-close</v-icon>
                    </v-btn>
                </v-card-actions>
            </v-card-title>

            <v-text-field class="ma-2" outlined dense v-model="search" append-icon="mdi-magnify" label="Search" single-line
                hide-details></v-text-field>
            <v-divider></v-divider>
            <v-data-table class="px-2" :headers="headers" :items="ruleFunctions" @click:row="showInsertPopup"
                :search="search"
                :footer-props="{ itemsPerPageOptions: [5, 10, 15], itemsPerPageText: 'Functions per page' }">
            </v-data-table>
        </v-card>
    </v-dialog>
</template>
    
  
    
<script>
import functionsJson from "@/assets/ruleFunctions.json"
export default {
    props: {
        allFunctionsPopup: Object // data about All functions popup {visible, onlyShow, ruleIndex, cursorPosition}
    },
    data() {
        return {
            search: "", // for storing current search 
            headers: [ // headers for the table, also important for searching
                { text: "Name", value: "name" },
                { text: "Function", value: "sign", align: "center" },
                { text: "Description", value: "description" },
                { text: "Example", value: "example", align: "center" },
                { text: "Input", value: "input" },
                { text: "Output", value: "output" }
            ],
            ruleFunctions: functionsJson // information about functions from JSON file
        }
    },
    methods: {
        /**
         * Method which hides the all functions popup (emits function in parent)
        */
        hidePopup() {
            this.$emit("hide-all-functions-popup")
        },
        /**
         * Method which shows insert popup after clicking specific function
         * @param {Object} row Clicked row in All functions popup
         */
        showInsertPopup(row) {
            // Rows are not clickable when only Showing functions
            if (!this.allFunctionsPopup.onlyShow) {
                const functionIndex = this.ruleFunctions.findIndex((obj) => obj === row);
                // assign which function should be inserted, to which rule and position of the cursor in a rule
                const data = {
                    functionIndex: functionIndex,
                    ruleIndex: this.allFunctionsPopup.ruleIndex,
                    cursorPosition: this.allFunctionsPopup.cursorPosition
                }
                this.$emit("hide-all-functions-popup")
                this.$emit("show-insert-popup", data)
            }
        }
    }
}
</script>
  
  
<style>
.close-btn {
    position: absolute;
    top: 12px;
    right: 12px;
}
</style>
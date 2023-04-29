<!--
   * Author : Jiri Mladek
   * Licence: MIT, see LICENSE
-->

<template>
    <v-dialog v-model="functionsInsertPopup.visible" max-width="1200">
        <v-card>
            <v-card-title justify-center class="headline primary">
                {{ ruleFunction.name }}
                <v-spacer></v-spacer>
                <v-card-actions>
                    <v-btn class="close-btn" color="grey darken-4" fab x-small dark @click="hidePopup()">
                        <v-icon>mdi-close</v-icon>
                    </v-btn>
                </v-card-actions>
            </v-card-title>
            <v-container>
                <v-row>
                    <v-col>
                        <p class="font-weight-medium my-0 text-h6">
                            {{ ruleFunction.description }}
                        </p>
                    </v-col>
                </v-row>
                <v-divider class="my-2 grey darken-1"></v-divider>
                <v-row>
                    <v-col cols="12">
                        <v-sheet outlined color="grey lighten-1" rounded>
                            <v-card>
                                <v-container>
                                    <v-row align="center" v-for="(operand, index) in ruleFunction.operands" :key="index">
                                        <v-col cols="auto">
                                            <p class="mb-0">
                                                {{ operand.specification }} :
                                            </p>
                                        </v-col>
                                        <v-col class="px-0">
                                            <v-text-field v-model="functionOperands[index]" type="text" maxlength="1" dense
                                                required :style="{ width: '5ch' }" outlined class="py-0 operandField"
                                                hide-details></v-text-field>
                                        </v-col>
                                    </v-row>
                                </v-container>
                            </v-card>
                        </v-sheet>
                    </v-col>
                </v-row>
                <v-row>
                    <v-col cols="12" v-show="!textFieldValid" class="py-0">
                        <v-alert dense class="mb-0" type="error">
                            Invalid operands!
                        </v-alert>
                    </v-col>
                </v-row>
                <v-row>
                    <v-col align="center">
                        <v-btn color="primary black--text" @click="insertFunction()">
                            Insert
                        </v-btn>
                    </v-col>

                </v-row>
            </v-container>
        </v-card>
    </v-dialog>
</template>



<script>
import functionsJson from "@/assets/ruleFunctions.json"

export default {
    data() {
        return {
            ruleFunctions: functionsJson, // data about each rule function
            functionOperands: [], // array for storing operands for specific rule function
            textFieldValid: true // boolean indicating if operands text fields are valid
        }
    },
    props: {
        functionsInsertPopup: Object, // data about Function insert popup {visible, functionIndex, ruleIndex, cursorPosition}
    },
    methods: {
        /**
         * Method which hides the Insert popup (emits function in parent)
        */
        hidePopup() {
            this.$emit("hide-insert-popup");
        },
        /**
         * Method which creates a function from function sign and operands
         */
        insertFunction() {
            // enable inserting only if all operands are valid
            if (!(this.textFieldValid = this.validateOperands())) {
                return;
            }
            const operandsCount = this.ruleFunction.operands.length;
            const functionSign = this.ruleFunction.sign.slice(0, -operandsCount) // get the function sign, remove the abstract operands
            const finalFunction = functionSign + this.functionOperands.join("");
            this.hidePopup();
            this.$emit("add-function", finalFunction); // add function to specific rule in a parent
        },
        /**
         * Method which validates all operands of rule function based on data type (and corresponding regex)
         */
        validateOperands() {
            for (let index = 0; index < this.functionOperands.length; index++) {
                const pattern = (this.ruleFunction.operands[index].type === 'int') ? /^[0-9]{1}$/ : /^[^\s]{1}$/;
                if (!pattern.test(this.functionOperands[index])) { // when any operand is not valid
                    return false;
                }
            }
            return true;
        }
    },
    computed: {
        /**
         * Computed property to get data of selected rule function
         */
        ruleFunction() {
            return this.ruleFunctions[this.functionsInsertPopup.functionIndex]
        }
    },
    watch: {
        /**
         * Initializes the operands array to empty strings when popup is shown, because of validation
         * @param {Object} newData New data for Insert popup
         */
        functionsInsertPopup(newData) {
            if (newData.visible === true) {
                this.textFieldValid = true;
                const numOfOperands = this.ruleFunction.operands.length;
                this.functionOperands = Array(numOfOperands).fill("");
            }
        }
    }
};
</script>

<style scoped>
.operandField>>>input {
    text-align: center
}
</style>
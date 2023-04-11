
<template>
    <v-dialog v-model="functionsInsertPopup.visible" max-width="1200">
        <v-card>
            <v-card-title justify-center class="headline orange lighten-3">
                Insert a function
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
                        <p class="font-weight-medium">
                            Function:
                        </p>
                        <v-textarea dense readonly hide-details auto-grow v-model="ruleFunction.name" outlined rows="1"
                            row-height="15" background-color="orange lighten-3" color="black">
                        </v-textarea>

                    </v-col>
                    <v-col>
                        <p class="font-weight-medium my-0">
                            Description:
                        </p>
                        <v-textarea dense readonly hide-details auto-grow v-model="ruleFunction.description" outlined
                            rows="1" row-height="10">
                        </v-textarea>

                    </v-col>
                </v-row>
                <v-divider class="my-2 grey darken-1"></v-divider>
                <v-alert v-if="!textFieldValid" type="error" tile>
                    Invalid operands!
                </v-alert>
                <v-row>
                </v-row>
                <v-row>
                    <v-col cols="9">
                        <p class="font-weight-medium my-0">
                            Operands:
                        </p>
                        <div class="operandsWrapper">
                            <v-row v-for="(operand, index) in ruleFunction.operands" :key="index">
                                <v-col md="auto">
                                    <p class="py-0">
                                        {{ operand.specification }} :
                                    </p>
                                </v-col>
                                <v-col>
                                    <v-text-field v-model="functionOperands[index]" type="text" maxlength="1" dense required
                                        :style="{ width: '5ch' }" outlined class="text-center py-0 operandField"
                                        hide-details></v-text-field>
                                </v-col>
                            </v-row>
                        </div>
                    </v-col>
                    <v-col cols="3" class="d-flex justify-end align-end">
                        <v-btn color="orange darken-3" depressed @click="insertFunction()">
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
            ruleFunctions: functionsJson,
            functionOperands: [],
            textFieldValid: true
        }
    },
    props: {
        functionsInsertPopup: Object,
    },
    methods: {
        hidePopup() {
            this.$emit("hide-insert-popup", false);
        },
        insertFunction() {
            if (!(this.textFieldValid = this.validateOperands())) {
                return;
            }
            const operandsCount = this.ruleFunction.operands.length;
            const functionSign = this.ruleFunction.sign.slice(0, -operandsCount) //get the function sign, remove the abstract operands
            const finalFunction = functionSign + this.functionOperands.join("");
            this.hidePopup();
            this.$emit("update-rule", finalFunction);
        },
        validateOperands() {
            for (let index = 0; index < this.functionOperands.length; index++) {
                const pattern = (this.ruleFunction.operands[index].type === 'int') ? /^[0-9]{1}$/ : /^[^\s]{1}$/;
                if (!pattern.test(this.functionOperands[index])) {
                    return false;
                }
            }
            return true;
        }
    },
    computed: {
        ruleFunction() {
            return this.ruleFunctions[this.functionsInsertPopup.functionIndex]
        }
    },
    watch: {
        /**Function which initializes the operands array to empty strings when popup is shown, because of validation */
        functionsInsertPopup(newObject) {
            if (newObject.visible === true) {
                this.textFieldValid = true;
                const numOfOperands = this.ruleFunction.operands.length;
                this.functionOperands = Array(numOfOperands).fill("");
            }
        }
    }
};
</script>

<style scoped>
.operandsWrapper {
    border: 1px solid rgb(100, 100, 100);
    border-top-right-radius: 4px;
    border-bottom-right-radius: 4px;
    border-top-left-radius: 4px;
    border-bottom-left-radius: 4px;
    padding: 10px 5px;
}
</style>
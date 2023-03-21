
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

                <v-row v-if="ruleFunction.operands.length">
                    <v-col cols="9">
                        <p class="font-weight-medium my-0">
                            Operands:
                        </p>

                        <div class="operandsWrapper">
                            <v-row v-for="(operand, index) in ruleFunction.operands">
                                <v-col md="auto">
                                    <p class="py-0">
                                        {{ operand.specification }} :
                                    </p>
                                </v-col>
                                <v-col>
                                    <v-text-field v-model="functionOperands[index]" dense required outlined autofocus
                                        hide-details class="py-0"></v-text-field>
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
            functionOperands: []
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
            this.hidePopup();
            const operandsCount = this.ruleFunction.operands.length;
            const functionSign = this.ruleFunction.sign.slice(0, -operandsCount) //get the function sign, remove the abstract operands
            const finalFunction = " " + functionSign + this.functionOperands.join(""); //
            this.$emit("update-rule", finalFunction);
        }
    },
    computed: {
        ruleFunction() {
            return this.ruleFunctions[this.functionsInsertPopup.functionIndex]
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
<template>
    <v-sheet rounded :elevation="20">
        <v-chip label class="ma-1" color="primary" @mousedown="showInsertPopup(1)">
            Lowercase
        </v-chip>
        <v-chip label class="ma-1" color="primary" @mousedown="showInsertPopup(2)">
            Uppercase
        </v-chip>
        <v-chip label class="ma-1" color="primary" @mousedown="showInsertPopup(3)">
            Capitalize
        </v-chip>
        <v-chip label class="ma-1" color="primary" @mousedown="showInsertPopup(13)">
            Append
        </v-chip>
        <v-chip label class="ma-1" color="primary" @mousedown="showInsertPopup(14)">
            Prepend
        </v-chip>
        <v-chip label class="ma-1" color="primary" @mousedown="showAllFunctionsPopup(false)">
            Add other function
        </v-chip>
    </v-sheet>
</template>



<script>
export default {
    props: {
        ruleIndex: Number,
        ruleLineRef: Array
    },
    methods: {
        showInsertPopup(functionIndex) {
            const data = {
                functionIndex: functionIndex,
                ruleIndex: this.ruleIndex,
                cursorPosition: this.cursorPosition
            }
            this.$emit("show-insert-popup", data)
        },
        showAllFunctionsPopup(onlyShow) {
            const data = {
                visible: true,
                onlyShow: onlyShow,
                ruleIndex: this.ruleIndex,
                cursorPosition: this.cursorPosition
            }
            this.$emit("show-all-functions-popup", data)
        }
    },
    computed:{
        /**
         * Gets the position, where the function should be inserted
         */
        cursorPosition(){
            const inputElement = this.ruleLineRef[0].$el.querySelector('input');
            const position = inputElement.selectionStart;
            return position;
        }
    }
};
</script>

<style></style>
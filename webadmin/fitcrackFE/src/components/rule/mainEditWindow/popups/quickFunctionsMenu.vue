<!--
   * Author : Jiri Mladek
   * Licence: MIT, see LICENSE
-->

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
        ruleIndex: Number, // index of the editing rule
        ruleLineRef: Array // reference to HTML element of the editing rule
    },
    methods: {
        /**
         * Method which assigns required data of Insert popup when specific function in a Quick menu is clicked
         * @param {Number} functionIndex Index of the rule function
         */
        showInsertPopup(functionIndex) {
            
            const data = {
                functionIndex: functionIndex,
                ruleIndex: this.ruleIndex,
                cursorPosition: this.cursorPosition
            }
            this.$emit("show-insert-popup", data) // propagate into parent
        },
        /**
         * Method which assigns required data of All functions popup when 'Add other function' button in a Quick menu is clicked
         * @param {Boolean} onlyShow Flag indicating if the popup with all functions should be clickable or readonly
         */
        showAllFunctionsPopup(onlyShow) {
            const data = {
                visible: true,
                onlyShow: onlyShow,
                ruleIndex: this.ruleIndex,
                cursorPosition: this.cursorPosition
            }
            this.$emit("show-all-functions-popup", data) // propagate into parent
        }
    },
    computed:{
        /**
         * Gets the current cursor position, where the function should be inserted
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
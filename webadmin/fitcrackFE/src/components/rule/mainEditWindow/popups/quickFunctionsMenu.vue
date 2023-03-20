<template>
    <v-sheet rounded :elevation="20" >
        <v-chip label class="ma-1" color="primary" @mousedown="showInsertPopup(1)">
            Lowercase
        </v-chip>
        <v-chip label class="ma-1" color="primary"  @mousedown="showInsertPopup(2)">
            Uppercase
        </v-chip>
        <v-chip label class="ma-1" color="primary" @mousedown="showInsertPopup(3)">
            Capitalize
        </v-chip>
        <v-chip label class="ma-1" color="primary" @mousedown="showInsertPopup(15)">
            Append
        </v-chip>
        <v-chip label class="ma-1" color="primary" @mousedown="showInsertPopup(16)">
            Prepend
        </v-chip>
        <v-chip label class="ma-1" color="primary" @mousedown="showAllFunctionsPopup(false)">
            Add other function
        </v-chip>
    </v-sheet>
</template>



<script>
export default {
    data() {
        return {
            showFunctionsPopup: false
        }
    },
    methods: {
        updateFunctionsPopupState(updatedState) {
            this.showFunctionsPopup = updatedState;
        },
        showInsertPopup(functionIndex){
            const data = {
                visible: true,
                functionIndex: functionIndex
            }
            this.$emit("show-insert-popup", data)
        },
        showAllFunctionsPopup(onlyShow){
            const data = {
                visible: true,
                onlyShow: onlyShow
            }
            this.$emit("show-all-functions-popup", data)
        }
    }
};
</script>

<style>

</style>
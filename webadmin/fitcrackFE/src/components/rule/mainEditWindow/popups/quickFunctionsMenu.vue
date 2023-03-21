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
        <v-chip label class="ma-1" color="primary" @mousedown="showInsertPopup(13)">
            Append
        </v-chip>
        <v-chip label class="ma-1" color="primary" @mousedown="showInsertPopup(19)">
            Prepend
        </v-chip>
        <v-chip label class="ma-1" color="primary" @mousedown="showAllFunctionsPopup(false)">
            Add other function
        </v-chip>
    </v-sheet>
</template>



<script>
export default {
    props:{
        ruleIndex: Number
    },
    data() {
        return {
            
        }
    },
    methods: {
        showInsertPopup(functionIndex){
            const data = {
                visible: true,
                functionIndex: functionIndex,
                ruleIndex: this.ruleIndex
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
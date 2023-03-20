
<template>
    <v-dialog v-model="functionsInsertPopup.visible" max-width="1200">
        {{ functionsInsertPopup.functionIndex }}
        <v-btn class="close-btn" color="grey darken-4" fab x-small dark @click="hidePopup()">
            <v-icon>mdi-close</v-icon>
        </v-btn>
    </v-dialog>
</template>



<script>
export default {
    props: {
        functionsInsertPopup: Object
    },
    methods:{
        hidePopup(){
            this.$emit("hide-insert-popup", false);
        }
    }
};
</script>

<style></style>
<template>
    <v-sheet rounded :elevation="20" class="quickFunctionsMenuPopup">
        <v-chip label class="ma-1" color="primary">
            Lowercase
        </v-chip>
        <v-chip label class="ma-1" color="primary">
            Uppercase
        </v-chip>
        <v-chip label class="ma-1" color="primary">
            Capitalize
        </v-chip>
        <v-chip label class="ma-1" color="primary">
            Append
        </v-chip>
        <v-chip label class="ma-1" color="primary">
            Prepend
        </v-chip>
        <v-chip label class="ma-1" color="primary" @mousedown="showFunctionsPopup=true">
            Add other function
        </v-chip>
        <ruleFunctions ruleFunctions v-bind:showFunctionsPopup="showFunctionsPopup"
            v-on:update-functions-popup-state="updateFunctionsPopupState" :readonly="false"></ruleFunctions>
    </v-sheet>
</template>



<script>
import ruleFunctions from '@/components/rule/mainEditWindow/popups/ruleFunctions.vue';
export default {
    data() {
        return {
            showFunctionsPopup: false
        }
    },
    methods: {
        updateFunctionsPopupState(updatedState) {
            this.showFunctionsPopup = updatedState;
        }
    },
    components: {
        ruleFunctions
    }

};
</script>

<style>
.quickFunctionsMenuPopup {
    position: absolute;
    bottom: 105%;
    max-width: 120%;
    padding: 0;
}
</style>
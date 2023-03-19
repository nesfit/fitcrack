<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
    <v-dialog v-model="showPopup" max-width="1200">
        <v-card>
            <v-card-title justify-center class="headline orange lighten-3">
                <div v-if="readonly">
                    Rule functions
                </div>
                <div v-else>
                    Choose a rule function
                </div>
                <v-spacer></v-spacer>
                <v-card-actions>
                    <v-btn class="close-btn" color="grey darken-4" fab x-small dark @click="hideFunctionsPopup()">
                        <v-icon>mdi-close</v-icon>
                    </v-btn>
                </v-card-actions>
            </v-card-title>

            <v-text-field class="ma-2" outlined dense v-model="search" append-icon="mdi-magnify" label="Search" single-line
                hide-details></v-text-field>
            <v-divider></v-divider>
            <v-data-table :headers="headers" :items="ruleFunctions.functions" :search="search"
                :footer-props="{ itemsPerPageOptions: [5, 10, 15], itemsPerPageText: 'Functions per page' }">
            </v-data-table>
        </v-card>
    </v-dialog>
</template>
    
  
    
<script>
import functionsJson from "@/assets/ruleFunctions.json"
export default {
    props: {
        readonly: Boolean,
        showFunctionsPopup: Boolean
    },
    data() {
        return {

            search: "",
            headers: [
                { text: "Name", value: "name" },
                { text: "Function", value: "sign", align: "center" },
                { text: "Description", value: "description" },
                { text: "Example", value: "example" },
                { text: "Input", value: "input" },
                { text: "Output", value: "output" }
            ],
            ruleFunctions: functionsJson
        }
    },
    methods:{
        hideFunctionsPopup(){
            this.$emit("update-functions-popup-state", false)
        }
    },
    computed:{
        showPopup(){
            return this.showFunctionsPopup;
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
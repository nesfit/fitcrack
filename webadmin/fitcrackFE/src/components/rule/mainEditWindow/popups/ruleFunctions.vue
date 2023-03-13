<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
    <v-dialog v-model="dialog" max-width="1200">
        <template v-slot:activator="{ on, attrs }">
            <v-btn color="grey lighten-1" v-bind="attrs" v-on="on">
                Show rule functions
            </v-btn>
        </template>

        <v-card>
            <v-card-title justify-center class="headline orange lighten-3">
                Rule functions
                <v-spacer></v-spacer>
                <v-card-actions>
                    <v-btn icon class="close-btn" color="black" @click="dialog = false">
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
import json from "@/assets/ruleFunctions.json"
export default {
    data() {
        return {
            dialog: false,
            search: "",
            headers: [
                { text: "Name", value: "name"},
                { text: "Function", value: "sign", align: "center"},
                { text: "Description", value: "description" },
                { text: "Example", value: "example" },
                { text: "Input", value: "input" },
                { text: "Output", value: "output" }
            ],
            ruleFunctions: json
        }
    }
}
</script>
  
  
<style>
.close-btn {
    position: absolute;
    top: 10px;
    right: 10px;
}


</style>
<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
    <v-dialog v-model="popupVisible">
        <v-card>
            <v-card-title class="headline orange lighten-3">
                Append dictionary
            </v-card-title>
            <v-tabs v-model="tab">
                <v-tab>
                    From filesystem
                </v-tab>
                <v-tab>
                    From server
                </v-tab>
            </v-tabs>
            <v-tabs-items v-model="tab">
                <v-tab-item>
                    <v-card text class="text-center py-2">
                        <v-file-input accept=".txt, .dict" outlined chips show-size class="pr-2" label="Select file"
                            @change="onDictionaryChange" />
                        <v-btn color="primary" outlined :disabled="systemFileSelected === false" @click="appendSystemDict">
                            Append
                            <v-icon right>
                                mdi-plus
                            </v-icon>
                        </v-btn>
                    </v-card>
                </v-tab-item>
                <v-tab-item>
                    <v-card text class="text-center py-2">
                        <dictSelector v-model="serverFileSelected"> </dictSelector>
                        <v-btn color="primary" outlined :disabled="serverFileSelected.length == 0"
                            @click="appendServerDict">
                            Append
                            <v-icon right>
                                mdi-plus
                            </v-icon>
                        </v-btn>
                    </v-card>
                </v-tab-item>
            </v-tabs-items>
        </v-card>
    </v-dialog>
</template>
  
    
<script>
import dictSelector from '@/components/selector/dictionarySelector.vue'

export default {
    props: {
        value: Boolean, // for visibility of popup
        allPasswordsString: String // all passwords concatenated
    },
    data() {
        return {
            updatedAllPasswordsString: "", // string for storing updated passwords after append
            tab: null, // variable for v-tab choice
            systemFileSelected: false, // boolean to indicate that file from filesystem was selected
            serverFileSelected: [] // array to store selected file from server
        };
    },
    methods: {
        /**
         * Method which gets the content of selected dictionary from filesystem and concatenates it with existing passwords
         * @param {*} event Selected file from filesystem 
         */
        onDictionaryChange(event) {
            // if file is selected
            if (event) {
                const reader = new FileReader();
                reader.onload = (event) => {
                    this.updatedAllPasswordsString = this.allPasswordsString.concat(reader.result);  // concatenate passwords from new file with existing ones
                };
                reader.readAsText(event)
                this.systemFileSelected = true;
            }
            else {
                this.systemFileSelected = false;
            }
        },
        /**
         * Method which appends the dictionary (from client filesystem) after clicking the button
         */
        appendSystemDict() {
            this.$emit("update-passwords", this.updatedAllPasswordsString) //update passwords in parent
            this.systemFileSelected = false;
            this.popupVisible = false;
        },
        /**
         * Method which appends the content of selected dictionary (from server)
         */
        appendServerDict() {
            // get content of selected dictionary
            this.axios.get(this.$serverAddr + "/dictionary/" + this.serverFileSelected[0].id + "/download").then((response) => {
                this.updatedAllPasswordsString = this.allPasswordsString.concat(response.data) // concatenate passwords from new file with existing ones
                this.$emit("update-passwords", this.updatedAllPasswordsString) // update passwords in parent
                this.serverFileSelected = []
                this.popupVisible = false;
            });
        }
    },
    computed: {
        // getter and setter for visibility of popup
        popupVisible: {
            get() {
                return this.value;
            },
            set(visibility) {
                this.$emit('input', visibility);
            }
        }
    },
    components: {
        dictSelector
    }

};
</script>
  
  
<style></style>
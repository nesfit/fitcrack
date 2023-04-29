<!--
   * Author : Jiri Mladek
   * Licence: MIT, see LICENSE
-->

<template>
    <v-col cols="10" md="4">
        <v-sheet outlined color="grey lighten-1" rounded>
            <v-card>
                <v-card-title>
                    <v-row justify="center" class="text-center border-down boxTitle text-h5 font-weight-medium py-2">
                        Live mangled passwords preview
                    </v-row>
                </v-card-title>
                <v-container>

                    <v-row>
                        <v-col class="font-weight-medium">
                            Type passwords or append a dictionary:
                        </v-col>
                    </v-row>
                    <v-row>
                        <v-col class="py-0">
                            <v-btn class="px-2" color="orange lighten-3 black--text" small 
                                @click="appendDictPopup = true">
                                <v-icon left>
                                    mdi-file
                                </v-icon>
                                Append dictionary
                            </v-btn>
                            <appendDictPopup v-model="appendDictPopup" v-bind:allPasswordsString="allPasswordsString"
                                v-on:update-passwords="updatePasswords"></appendDictPopup>
                        </v-col>
                    </v-row>
                    <v-row>
                        <v-col class="pb-0">
                            <v-sheet outlined color="grey lighten-1" rounded>
                                <v-textarea solo hide-details class="textArea" label="Type or append passwords"
                                    :value="allPasswordsString" @input="updatePasswords"></v-textarea>
                            </v-sheet>
                        </v-col>
                    </v-row>
                    <v-row>
                        <v-col>
                            <v-btn class="py-0" color="red accent-2 black--text" small 
                                @click="updatePasswords('')">
                                <v-icon>
                                    mdi-delete
                                </v-icon>
                                Reset passwords
                            </v-btn>
                        </v-col>
                    </v-row>
                    <v-row>
                        <v-col>
                            <v-alert tile color="orange" text class="mb-0">
                                Maximum number of mangled passwords is set to {{ max_mangled_passwords }}. For change go to
                                <router-link :to="{ name: 'settings' }">
                                    <b>advanced settings</b>.
                                </router-link>
                            </v-alert>
                        </v-col>
                    </v-row>
                    <v-row>
                        <v-col class="font-weight-medium">
                            Mangled passwords:
                        </v-col>
                    </v-row>
                    <v-row class="my-1">
                        <v-progress-linear v-visible="mangledPasswords.loading" indeterminate
                            color="primary"></v-progress-linear>
                    </v-row>
                    <v-row>
                        <v-col class="py-0">
                            <v-sheet outlined color="grey lighten-1" rounded>
                                <v-textarea class="textArea" hide-details readonly solo label="Mangled passwords preview"
                                    v-model="this.mangledPasswords.value"></v-textarea>
                            </v-sheet>
                        </v-col>
                    </v-row>
                    <v-row>
                        <v-col class="text-center pt-5">
                            <v-btn height="40" multi-line text-wrap class="primary black--text"
                                :disabled="mangledPasswords.loading" small @click="downloadMangledPasswords()">
                                Download <br>mangled passwords
                            </v-btn>
                        </v-col>
                    </v-row>
                </v-container>
            </v-card>
        </v-sheet>
    </v-col>
</template>


<script>
import appendDictPopup from '@/components/rule/mainEditWindow/popups/appendDictPopup.vue';

export default {
    props: {
        mangledPasswords: { // object with concatenated mangled passwords and boolean indicating generating
            type: Object,
            default: () => ({ value: "", loading: false })
        },
        allPasswordsString: String // string for storing all passwords concatenated
    },
    data() {
        return {
            appendDictPopup: false, // true if append dictionary popup should be shown, false if hidden
            max_mangled_passwords: 5000 // TODO
        }
    },
    methods: {
        /**
         * Method which emits updating passwords in parent
         * @param {String} updatedAllPasswordsString Updated passwords
         */
        updatePasswords(updatedAllPasswordsString) {
            this.$emit("update-passwords", updatedAllPasswordsString);
        },
        /**
         * Method which downloads mangled passwords as a text file
         */
        downloadMangledPasswords() {
            const blob = new Blob([this.mangledPasswords.value], { type: 'text/plain' }); // content of the file to be downloaded
            const url = URL.createObjectURL(blob);
            const link = document.createElement('a'); // create a link for download
            link.href = url;
            link.download = "mangledPasswords.txt" // give file a name
            link.click();
            URL.revokeObjectURL(url)
            this.$success("Successfully downloaded mangled passwords") // show success message
        }
    },
    mounted() {
        // get the maximum number of mangled passwords from database
        this.axios.get(this.$serverAddr + '/settings').then((response) => {
            this.max_mangled_passwords = response.data.max_mangled_passwords;
        });
    },
    components: {
        appendDictPopup
    }
};

</script>


<style>
.line-number {
    width: 32px;
    text-align: right;
    color: #888;
    font-family: monospace;
    font-size: 14px;
    line-height: 1.2;
}

.v-textarea textarea {
    white-space: pre;
    overflow-x: auto;
}</style>
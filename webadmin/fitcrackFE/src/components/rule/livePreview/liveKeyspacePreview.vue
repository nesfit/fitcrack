<template>
    <v-col cols="5" md="3" class="bordered">
        <v-container>
            <v-row justify="center" class="border-down boxTitle text-h5 py-2">
                Live mangled passwords preview
            </v-row>
            <v-row>
                <v-col class="font-weight-medium">
                    Type passwords or append a dictionary:
                </v-col>
            </v-row>
            <v-row>
                <v-col class="py-0">
                    <v-btn class="px-2" color="orange lighten-3" small depressed @click="appendDictPopup = true">
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
                    <v-textarea solo hide-details class="textArea" label="Type or append passwords" :value="allPasswordsString"
                        @input="updatePasswords"></v-textarea>
                </v-col>
            </v-row>
            <v-row>
                <v-col>
                    <v-btn class="py-0" color="red accent-2" small depressed @click="updatePasswords('')">
                        <v-icon>
                            mdi-delete
                        </v-icon>
                        Reset passwords
                    </v-btn>
                </v-col>

            </v-row>


            <v-row>

                <v-alert tile color="orange" text class="mb-0">
                    Maximum number of mangled passwords is set to {{ max_mangled_passwords }}. For change go to
                    <router-link :to="{ name: 'settings' }">
                        <b>advanced settings</b>.
                    </router-link>
                </v-alert>

            </v-row>
            <v-row>
                <v-col class="font-weight-medium">
                    Mangled passwords:
                </v-col>
            </v-row>
            <v-row class="my-1">
                <v-progress-linear v-visible="mangledPasswords.loading" indeterminate color="orange"></v-progress-linear>
            </v-row>
            <v-row>
                <v-col class="py-0">
                    <v-textarea class="textArea" readonly solo label="Mangled passwords preview"
                        v-model="this.mangledPasswords.value"></v-textarea>
                </v-col>
            </v-row>
            <v-row>
                <v-col class="text-center pt-0">
                    <v-btn height="40" multi-line text-wrap class="orange darken-3" :disabled="mangledPasswords.loading"
                        small @click="downloadFinalPasswords()">
                        Download <br>mangled passwords
                    </v-btn>
                </v-col>
            </v-row>
        </v-container>
    </v-col>
</template>


<script>
import appendDictPopup from '@/components/rule/mainEditWindow/popups/appendDictPopup.vue';
export default {
    props: {
        mangledPasswords: Object,
        allPasswordsString: String
    },
    data() {
        return {
            appendDictPopup: false,
            max_mangled_passwords: 5000
        }
    },
    methods: {
        updatePasswords(updatedAllPasswordsString) {
            this.$emit("update-passwords", updatedAllPasswordsString);
        },
        downloadFinalPasswords() {
            const blob = new Blob([this.mangledPasswords.value], { type: 'text/plain' });
            const url = URL.createObjectURL(blob);
            const link = document.createElement('a');
            link.href = url;
            link.download = "mangledPasswords.txt"
            link.click();
            URL.revokeObjectURL(url)
            this.$success("Successfully downloaded mangled passwords")
        }
    },
    components: {
        appendDictPopup
    },
    mounted() {
        // get the maximum number of mangled passwords from database
        this.axios.get(this.$serverAddr + '/settings').then((response) => {
            this.max_mangled_passwords = response.data.max_mangled_passwords;
        });
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
}
</style>
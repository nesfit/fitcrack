<template>
    <v-col cols="5" md="3" class="bordered">
        <v-container>
            <v-row justify="center" class="border-down boxTitle text-h5 py-2">
                Live keyspace preview
            </v-row>
            <v-row>
                <v-col class="font-weight-medium">
                    Type passwords or append a dictionary:
                </v-col>
            </v-row>
            <v-row>
                <v-col class="py-0">
                    <!--
                        <input type="file" ref="appendDictionary" style="display: none"
                        @change="onDictionaryFileChange($event)">
                    -->

                    <v-btn class="px-2" color="orange lighten-3" small depressed @click="appendDictPopup = true">
                        <v-icon left>
                            mdi-file
                        </v-icon>
                        Append dictionary
                    </v-btn>
                    <appendDictPopup v-model="appendDictPopup" v-bind:passwordsContent="passwordsContent"
                        v-on:update-passwords="updatePasswords"></appendDictPopup>
                </v-col>
            </v-row>
            <v-row>
                <v-col class="pb-0">
                    <v-textarea solo class="textArea" label="Dictionary Content" v-model="passwordsContent"></v-textarea>
                </v-col>
            </v-row>
            <v-row>
                <v-col cols="12" class="text-center pt-0 pb-8">
                    <v-btn class="orange darken-3" :loading="previewPasswords.loading" :disabled="previewPasswords.loading" @click="generatePreview">
                        <span v-if="!previewPasswords.loading">Generate Preview</span>
                    </v-btn>
                </v-col>

            </v-row>
            <v-row>
                <v-col class="font-weight-medium">
                    Final keyspace:
                </v-col>
            </v-row>
            <v-row>
                <v-col class="py-0">
                    <v-textarea class="textArea" readonly solo label="Passwords preview"
                        v-model="this.previewPasswords.string"></v-textarea>
                </v-col>
            </v-row>
            <v-row>
                <v-col class="text-center pt-0">
                    <v-btn height="40" multi-line text-wrap color=" grey lighten-1" small
                        @click="downloadFinalPasswords()">Download <br>mangled passwords</v-btn>
                </v-col>
            </v-row>
        </v-container>
    </v-col>
</template>


<script>
import appendDictPopup from '@/components/rule/mainEditWindow/popups/appendDictPopup.vue';
export default {
    props: {
        previewPasswords: Object
    },
    data() {
        return {
            passwordsContent: "p@ssW0rd",
            appendDictPopup: false
        }
    },
    methods: {
        updatePasswords(updatedPasswordsContent) {
            this.passwordsContent = updatedPasswordsContent;
        },
        generatePreview() {
            this.$emit("generate-preview", this.passwordsContent);
        },
        downloadFinalPasswords() {
            const blob = new Blob([this.previewPasswords.string], { type: 'text/plain' });
            const url = URL.createObjectURL(blob);
            const link = document.createElement('a');
            link.href = url;
            link.download = "finalPasswords.txt"
            link.click();
            URL.revokeObjectURL(url)
        }
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
}
</style>
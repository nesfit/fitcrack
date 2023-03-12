<template>
    <v-col cols="12" md="3" class="bordered">
        <v-container>
            <v-row justify="center" class="border-down boxTitle text-h5 py-2">
                Live keyspace preview
            </v-row>
            <v-row>
                <v-col cols="2">
                    <v-list>
                        <v-list-item v-for="(line, index) in lines" :key="index">
                            <v-list-item-content class="line-number">{{ index + 1 }}</v-list-item-content>
                        </v-list-item>
                    </v-list>
                </v-col>
                <v-col cols="10">
                    <v-textarea v-model="text" @input="updateLines" :rows="rows" :style="{ height: `${height}px` }"
                        hide-details></v-textarea>
                </v-col>
            </v-row>


            <v-row>

            </v-row>

            <v-row>
                <v-col>
                    <input type="file" label="Load Dictionary" @change="onDictionaryFileChange($event)">
                    <v-textarea label="Dictionary Content" v-model="dictionaryContent"></v-textarea>
                </v-col>
                <v-col>
                    <v-btn @click="generatePreview">Generate Preview</v-btn>
                    <v-textarea label="Preview Passwords" v-model="previewPasswordsString"></v-textarea>
                </v-col>

            </v-row>
        </v-container>
    </v-col>
</template>


<script>
import fcTextarea from '@/components/textarea/fc_textarea.vue'


export default {
    data() {
        return {
            dictionaryContent: null,
            passwordsList: null,
            applicatorResult: null,
            previewPasswords: [],
            previewPasswordsString: "",

            text: "",
            lines: [],
            rows: 3, // initial number of rows
            lineHeight: 20, // adjust to match your font size and line height
            height: 0,
        }
    },
    methods: {
        onDictionaryFileChange(event) {
            const file = event.target.files[0];
            const reader = new FileReader();
            reader.onload = (event) => {
                this.dictionaryContent = event.target.result;
                this.passwordsList = event.target.result.split("\n");
                this.passwordsList.pop();
            };
            reader.readAsText(file);
        },
        generatePreview() {
            const data = {
                rulesList: this.rulesList,
                passwordsList: this.passwordsList
            };
            this.axios.post(this.$serverAddr + "/rule/preview", data).then((response) => {
                this.applicatorResult = response.data.passwordsPreview;
                this.filterPreviewPasswords();
            }).catch((error) => {
                this.previewPasswords = error.message;
            });
        },
        filterPreviewPasswords() {
            this.previewPasswords = [];
            this.applicatorResult.forEach(element => {
                if (element.retCode >= 0) {
                    this.previewPasswords.push(element.finalPassword);
                }
            });
            this.previewPasswordsString = this.previewPasswords.join("\n");
            console.log(this.previewPasswords);
        },
        updateLines() {
            // split text into lines and update the line numbers
            this.lines = this.text.split("\n");
            this.rows = this.lines.length;
            this.height = this.rows * this.lineHeight;
        },
    },
    components: {
        fcTextarea
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
</style>
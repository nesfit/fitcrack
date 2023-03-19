<template>
    <v-col cols="5" md="3" class="bordered">
        <v-container>
            <v-row justify="center" class="border-down boxTitle text-h5 py-2">
                Live keyspace preview
            </v-row>
            <v-row>
                <v-col class="font-weight-medium">
                    Type words or select a dictionary:
                </v-col>
            </v-row>
            <v-row>
                <v-col class="py-0">
                    <input type="file" ref="appendDictionary" style="display: none"
                        @change="onDictionaryFileChange($event)">
                    <v-btn class="px-2" color="orange lighten-3" depressed @click="$refs.appendDictionary.click()">
                        <v-icon left>
                            mdi-file
                        </v-icon>
                        Append dictionary
                    </v-btn>
                </v-col>
            </v-row>
            <v-row>
                <v-col class="pb-0">
                    <v-textarea solo label="Dictionary Content" v-model="dictionaryContent"></v-textarea>
                </v-col>
            </v-row>
            <v-row>
                <v-col cols="12" class="text-center pt-0 pb-8">
                    <v-btn class="orange darken-3" @click="generatePreview">Generate Preview</v-btn>
                </v-col>

            </v-row>
            <v-row>
                <v-col class="font-weight-medium">
                    Final keyspace:
                </v-col>
            </v-row>
            <v-row>
                <v-col class="pt-0">
                    <v-textarea readonly solo label="Preview Passwords" v-model="this.previewPasswordsString"></v-textarea>
                </v-col>

            </v-row>
        </v-container>
    </v-col>
</template>


<script>

export default {
    props: {
        passwordsList: Array,
        previewPasswordsString: String
    },
    data() {
        return {
            dictionaryContent: null,
            passwordsListData: this.passwordsList
        }
    },
    methods: {
        onDictionaryFileChange(event) {
            const file = event.target.files[0];
            const reader = new FileReader();
            reader.onload = (event) => {
                this.dictionaryContent = event.target.result;
                this.passwordsListData = event.target.result.split("\n");
                this.passwordsListData.pop();
            };
            reader.readAsText(file);
        },
        generatePreview() {
            this.$emit("generate-preview", this.passwordsListData)
        }
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
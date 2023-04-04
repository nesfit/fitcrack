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
        value: Boolean,
        passwordsContent: String
    },
    data() {
        return {
            passwordsContentCopy: "",
            tab: null,
            systemFileSelected: false,
            serverFileSelected: []
        };
    },
    methods: {
        onDictionaryChange(event) {
            if (event) {
                const reader = new FileReader();
                reader.onload = (event) => {
                    this.passwordsContentCopy = this.passwordsContent.concat(reader.result);
                };
                reader.readAsText(event)
                this.systemFileSelected = true;
            }
            else {
                this.systemFileSelected = false;
            }

        },
        appendSystemDict() {
            this.$emit("update-passwords", this.passwordsContentCopy)
            this.systemFileSelected = false;
            this.popupVisible = false;
        },
        appendServerDict() {
            this.axios.get(this.$serverAddr + "/dictionary/" + this.serverFileSelected[0].id + "/download").then((response) => {
                this.passwordsContentCopy = this.passwordsContent.concat(response.data)
                this.$emit("update-passwords", this.passwordsContentCopy)
                this.serverFileSelected = []
                this.popupVisible = false;
            });
        }
    },
    computed: {
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
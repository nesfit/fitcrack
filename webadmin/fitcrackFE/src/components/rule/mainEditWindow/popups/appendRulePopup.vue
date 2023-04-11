<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
    <v-dialog v-model="popupVisible">
        <v-card>
            <v-card-title class="headline orange lighten-3">
                Append rule file
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
                        <v-file-input accept=".txt, .rule" outlined chips show-size class="pr-2" label="Select file"
                            @change="onRuleFileChange" />
                        <v-btn color="primary" outlined :disabled="systemFileSelected === false" @click="appendRules">
                            Append
                            <v-icon right>
                                mdi-plus
                            </v-icon>
                        </v-btn>
                    </v-card>
                </v-tab-item>
                <v-tab-item>
                    <v-card text class="text-center py-2">
                        <rulesSelector v-model="serverFileSelected"></rulesSelector>
                        <v-btn color="primary" outlined :disabled="serverFileSelected.length == 0"
                            @click="appendServerRules">
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
import rulesSelector from '@/components/selector/rulesSelector.vue'

export default {
    props: {
        value: Boolean,
        rules: Array
    },
    data() {
        return {
            updatedRules: [],
            tab: null,
            systemFileSelected: false,
            serverFileSelected: []
        };
    },
    methods: {
        onRuleFileChange(event) {
            if (event) {
                const reader = new FileReader();
                reader.onload = (event) => {
                    const lines = reader.result.split("\n");
                    const newRules = lines.map(line => ({ value: line, error: false }))
                    this.updatedRules = this.rules.concat(newRules);
                    this.updatedRules.pop();
                };
                reader.readAsText(event)
                this.systemFileSelected = true;
            }
            else {
                this.systemFileSelected = false;
            }

        },
        appendRules() {
            this.$emit("update-rules", this.updatedRules)
            this.systemFileSelected = false;
            this.popupVisible = false;
        },
        appendServerRules() {
            this.axios.get(this.$serverAddr + "/rule/" + this.serverFileSelected[0].id + "/download").then((response) => {
                const lines = response.data.split("\n");
                const newRules = lines.map(line => ({ value: line, error: false }))
                this.updatedRules = this.rules.concat(newRules);
                this.updatedRules.pop();
                this.$emit("update-rules", this.updatedRules)
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
        rulesSelector
    }

};
</script>
  
  
<style></style>